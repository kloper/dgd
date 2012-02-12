// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// stack.cpp -- linked list with dgd tracing
// 

#include <boost/exception/diagnostic_information.hpp> 

#include <dgd.h>

template<typename T>
class node 
{
public:
   typedef node<T> node_type;
   typedef T value_type;

   node(const value_type& val) : 
      m_value(val),
      m_prev(NULL),
      m_next(NULL)
   {}

   void prev(node_type *p) { m_prev = p; }
   void next(node_type *n) { m_next = n; }
   
   node_type *prev() const { return m_prev; }
   node_type *next() const { return m_next; }
   value_type& value() { return m_value; }

private:
   value_type m_value;
   node_type *m_prev;
   node_type *m_next;
};

template<typename T>
class stack
{
public:
   typedef typename node<T>::node_type node_type;
   typedef typename node<T>::value_type value_type;

   stack() : 
      m_head(NULL),
      m_tail(NULL),
      m_count(0)
   {}

   void push(const value_type& value) {
      node_type *node = new node_type(value);

      node->prev(m_tail);
      if(m_tail != NULL) {
         m_tail->next(node);
      } 

      m_tail = node;

      if(m_head == NULL) {
         m_head = node;
      }
      
      m_count++;
   }

   void pop() {
      if(m_tail == NULL)
         return;

      node_type *prev = m_tail->prev();
      delete m_tail;

      m_tail = prev;
      if(prev == NULL) {
         m_head = NULL;
      } else {
         prev->next(NULL);
      }
      m_count--;
   }

   unsigned int count() const { return m_count; }
   value_type &top() { return m_tail->value(); } // bug !

private:
   node_type *m_head;
   node_type *m_tail;
   unsigned int m_count;
};

struct item 
{
   int id;
   int tag;

   item(int a, int b) : id(a), tag(b) {}
};

std::ostream &operator<<(std::ostream &ostr, const item& val)
{
   ostr << "{" << std::endl
        << dgd::incr 
        << "id: " << val.id << std::endl
        << "tag: " << val.tag << std::endl
        << dgd::decr
        << "}";

   return ostr;
}

int main(int argc, char **argv)
{
   try {
      dgd::controller<char>::init(argc, argv);
   }
   catch(...) {
      std::cerr << "Unable to init dgd. Try --trace-help"
                << std::endl
                << boost::current_exception_diagnostic_information() << std::endl;
      return -1;
   }

   dgd_scope;

   stack<item> item_stack;

   item_stack.push(item(1,1));
   item_stack.push(item(2,1));

   std::cout << item_stack.top() << std::endl;

   dgd_echo(item_stack.top());

   item_stack.pop();

   dgd_echo(item_stack.top());

   dgd_logger << "Before last pop" << std::endl;

   item_stack.pop();

   dgd_logger << "Before bug exposure";

   dgd_echo(item_stack.top());

   return 0;
}

// 
// Local Variables:
// compile-command: "make stack.obj"
// End:
//


