#include "./stack.hpp"
using namespace logo::vm;
Stack::Stack(size_t variable_count){
    variables.resize(variable_count);
}
void Array::setItem(size_t index,const stackcell &sc){
    if(data.size()<index){
        data.resize(index+1);
    }
    data[index] = sc;
}
