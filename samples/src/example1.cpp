#include <iostream>
#include <list>

struct MyStruct {
      int my_field;
      
      MyStruct( int val ): my_field( val ) {}
};

std::ostream& operator << ( std::ostream& ostr, const MyStruct& data ) {
   ostr << "[" << data.my_field << "]";
   return ostr;
}

int main( int argc, char** argv ) {
   std::list<MyStruct> my_data;
   int i;

   for( i = 0; i < 10; i++ ) my_data.push_back( i );

   std::list<MyStruct>::const_iterator iter = my_data.begin();

   while( iter != my_data.end() ) std::cout << *iter++ << " ";
   std::cout << std::endl;

   return 0;
}
