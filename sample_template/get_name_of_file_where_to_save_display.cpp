#include "time.h"
#include "iostream"
#include "get_name_of_file_where_to_save_display.hpp"

std::string  get_name_of_file_where_to_save_display(){
	time_t lt;
	lt = time(NULL);
	std::string s=ctime(&lt);
	s[s.size()-1]='.';
	for(int i=0;i<s.size();i++){if(s[i]==' ') s[i]='_';}
	for(int i=0;i<s.size();i++){if(s[i]==':') s[i]='_';}
	s=s+"png";
	return s;
	}