#ifndef BITMAP_H
#define BITMAP_H

#define BITSHRINK(i) (i >> 6)
#define BITOFFSET(i) (i & 0x3f)

typedef struct _Bitmap{
	size_t _size;
	size_t * _data;
	size_t len;

	_Bitmap(unsigned long size, size_t type=0) {	
		this->len = BITSHRINK(size)+1;
		this->_size = size;
		//this->_data = new unsigned long [len];
		this->_data = (size_t *)malloc(sizeof(size_t) * this->len);

		if (type == 0) this->clear();
		else this->fill();
	
	}

	~_Bitmap() {
		if(this->_data != NULL) {
			free(this->_data);
			this->_data=NULL;
		}
	}

	void fill(){
		#pragma omp parallel for
		for (size_t i=0; i<this->len; i++) {
			this->_data[i] = 0xffffffffffffffff;
		}
		#pragma omp barrier
	}

	void clear(){
		#pragma omp parallel for
		for (size_t i=0; i<this->len; i++) {
			this->_data[i] = 0;
		}
		#pragma omp barrier
	}

	void clear(size_t start, size_t count) {
		size_t end = start + count;
		size_t s_range = BITSHRINK(start);
		size_t s_offset= BITOFFSET(start);
		size_t e_range = BITSHRINK(end);
		size_t e_offset= BITOFFSET(end);
		if(s_range != e_range) {
			#pragma omp parallel for
			for (size_t i = s_range+1 ; i < e_range; i++) {
				this->_data[i] = 0;
			}
			#pragma omp barrier
			
			__sync_fetch_and_and(this->_data+s_range, ((1ul<<s_offset) - 1));
			__sync_fetch_and_and(this->_data+e_range, ~((1ul<<e_offset) - 1));
		} else {
			#pragma omp parallel for
			for (size_t i = start; i < end; i++) {
				clean_bit(i);
			}
			#pragma omp barrier
		}
	}

	int count_bits(){
		int count = 0;
		#pragma omp parallel for reduction(+:count)
		for (size_t i=0; i<this->_size; i++) {
			if(this->get_bit(i)) count++;
		}
		#pragma omp barrier
		return count;
	}

	int count_bits(size_t start, size_t end){
		int count = 0;
		#pragma omp parallel for reduction(+:count)
		for (size_t i = start; i <= end; i++) {
			if(this->get_bit(i)) count++;
		}
		#pragma omp barrier
		return count;
	}

	void set_bits(_Bitmap * bitmap, size_t start, size_t count) {
		size_t end = start + count;
		size_t s_range = BITSHRINK(start);
		size_t s_offset= BITOFFSET(start);
		size_t e_range = BITSHRINK(end);
		size_t e_offset= BITOFFSET(end);
		if(s_range != e_range) {
			#pragma omp parallel for
			for (size_t i = s_range+1 ; i < e_range; i++) {
				this->_data[i] = bitmap->_data[i];
			}
			#pragma omp barrier

			//s_range left
			size_t s_left = (s_range+1) << 6;
			#pragma omp parallel for
			for (size_t i = start; i < s_left; i++) {
				if (bitmap->get_bit(i)) set_bit(i);
				else clean_bit(i);
			}
			#pragma omp barrier
			#pragma omp parallel for
			for (size_t i = e_range << 6; i < end; i++) {
				if (bitmap->get_bit(i)) set_bit(i);
				else clean_bit(i);
			}
			#pragma omp barrier
			//e_range left
			
		} else {
			#pragma omp parallel for
			for (size_t i = start; i < end; i++) {
				if (bitmap->get_bit(i)) set_bit(i);
				else clean_bit(i);
			}
			#pragma omp barrier
		}
	}

	bool get_bits(size_t start, size_t count) {
		size_t end = start + count;
		size_t s_range = BITSHRINK(start);
		size_t s_offset= BITOFFSET(start);
		size_t e_range = BITSHRINK(end);
		size_t e_offset= BITOFFSET(end);
		if (s_range != e_range) {
			for (size_t i = s_range + 1; i < e_range; i++) {
				if (this->_data[i] != 0) { return true; }
			}

			long s = this->_data[s_range];
			long e = this->_data[e_range];
			if ((s & ~((1ul<<s_offset) - 1)) > 0) return true;
			if ((e &  ((1ul<<e_offset) - 1)) > 0) return true; 
		} else {
			for (size_t i = start; i < end; i ++) {
				if (get_bit(i)) { return true; }
			}
		}
		return false;
	}

	//BUG: COPY ERROR
	void copy_bits2(_Bitmap * b, size_t start, size_t end) {
		#pragma omp parallel for
		for (size_t i = start; i <= end; i++) {
			if (b->get_bit(i)) {
				this->set_bit(i);
			} else {
				this->clean_bit(i);
			}
		}
		#pragma omp barrier
	}

	void copy_bits(_Bitmap * b, size_t start, size_t count) {
		size_t end = start + count;
		size_t s_range = BITSHRINK(start);
		size_t s_offset= BITOFFSET(start);
		size_t e_range = BITSHRINK(end);
		size_t e_offset= BITOFFSET(end);
		size_t s = s_range, e = e_range - 1;
		if (s_offset == 0) s = s_range;
		else s = s_range + 1;

		#pragma omp parallel for
		for (size_t i = s; i <= e; i++) {
			this->_data[i] = b->_data[i];
		}
		#pragma omp barrier

		//Set left
		if (s < this->len - 1) {
			size_t left = (s+1) << 6;
			#pragma omp parallel for
			for (size_t i = s_offset; i < left; i++) {
				if (b->get_bit(i)) {
					this->set_bit(i);
				} else {
					this->clean_bit(i);
				}
			}
			#pragma omp barrier
		}
		//Set right
		if (e > 0) {
			size_t right = (e - 1) << 6;
			#pragma omp parallel for
			for (size_t i = right; i < e_offset; i++) {
				if (b->get_bit(i)) {
					this->set_bit(i);
				} else {
					this->clean_bit(i);
				}
			}
			#pragma omp barrier
		}
	}

	bool get_bit(size_t i) {
		return bool(this->_data[BITSHRINK(i)] & (1ul << BITOFFSET(i)));
	}

	void set_bit(size_t i) {
		__sync_fetch_and_or(this->_data+BITSHRINK(i), 1ul<<BITOFFSET(i));
	}

	void clean_bit(size_t i) {
		__sync_fetch_and_and(this->_data+BITSHRINK(i), ~(1ul<<BITOFFSET(i)));
	}
}Bitmap;

#endif
