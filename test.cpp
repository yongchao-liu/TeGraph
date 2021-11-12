#include<set>
#include<cstdio>
#include<algorithm>
using namespace std;
int main()
{
    set<pair<int,int> > s;
    s.insert(make_pair(1,2));
    s.insert(make_pair(1,3));
    s.insert(make_pair(2,1));
    set<pair<int,int> >::iterator it;
    for(it=s.begin();it!=s.end();it++){
        printf("%d %d\n",it->first,it->second);
    }

}
