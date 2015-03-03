#ifndef __property_t_hpp__
#define __property_t_hpp__

#define make_property(type, name)\
public:\
type name() const\
{\
   return name ## _;\
}\
void name(type new_val)\
{\
   name ## _ = new_val;\
}\
private:\
type name ## _


#endif
