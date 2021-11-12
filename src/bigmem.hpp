#ifndef BIGMEM_H
#define BIGMEM_H

#include "src/type.hpp"

template <typename T>
struct BigMem{
	size_t length = 0;
	long buffer_bytes = 0;
	long buffer_count = 0;
	long buffer_size = IOBUFFER;

	//MMap
	T * data_in_memory = NULL;
	std::vector<std::pair<T *, long> > blocks;

	inline BigMem(long _length){
		this->length = _length;
		this->buffer_bytes = _length * sizeof(T);
		data_in_memory = (T *)mmap(0, this->buffer_bytes + PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		this->reset(IOBUFFER);
	}

	inline ~BigMem(){
		assert(munmap(data_in_memory, length * sizeof(T)) == 0);
	}

	//Functions
	inline void reset(long _buffer_size) {
		this->buffer_size = 
			(_buffer_size > this->buffer_bytes ? this->buffer_bytes : _buffer_size);
		long buffer_unit = _buffer_size / sizeof(T);

		this->buffer_count = (long)ceil((double)this->buffer_bytes / (double)this->buffer_size);
		this->blocks.clear();

		for (int i = 0; i < this->buffer_count; i++) {
			if (i != this->buffer_count - 1) {
				this->blocks.emplace_back(data_in_memory + i * buffer_unit, (long)buffer_unit);
			} else {
				this->blocks.emplace_back(data_in_memory + i * buffer_unit, (long)(length - i * buffer_unit));
			}
		}

		//printf("%ld\n", this->buffer_size);
	}


	T & operator[](size_t i){
		return data_in_memory[i];
	}

	inline void load(std::string input){
		int fd = open(input.c_str(), O_RDONLY);
		
		long bytes;
		long offset = 0;
		int count = 0;
		while (true) {
			bytes = pread(fd, (char *)data_in_memory + offset, IOBUFFER, offset);
			if (bytes <= 0) break;

			count ++;
			offset += bytes;
		}

		close(fd);
	}

	inline void save(std::string output, VertexNum vc){
		this->reset(IOBUFFER);

		if (file_exists(output)) remove_directory(output);
		create_directory(output);

		char filename[256];
		sprintf(filename, "%s/block-0", output.c_str());
		int fout = open(filename, O_WRONLY | O_CREAT, 0644);
		assert(fout != -1);

		for (int i = 0; i < buffer_count; i++) {
			std::pair<T *, long> data = this->blocks[i];

			long bytes = write(fout, (char *)data.first, data.second * sizeof(T));
			assert(bytes == data.second * sizeof(T));
		}

		close(fout);

		//write ini file
		sprintf(filename, "%s/graph.ini", output.c_str());
		FILE * fd_ini = fopen(filename, "w");
		fprintf(fd_ini, "%d %d %ld %d %d\n", 1, vc, this->length, 0, 0);
		fprintf(fd_ini, "%d %d %ld", 0, vc - 1, this->length);
		fclose(fd_ini);
	}

	inline std::pair<T *, long> & get_block(int i){
		return this->blocks[i];
	}
};

#endif