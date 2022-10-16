// Interfaces.h
// Copyright: rofl0r  License: GNU GPLv3
// After: http://www.codeguru.com/cpp/cpp/cpp_mfc/oop/article.php/c9989

#ifndef INTERFACES_H_
#define INTERFACES_H_


#define Interface class

#define DeclareInterface(name) Interface name { \
          public: \
          virtual ~name() {}

#define DeclareBasedInterface(name, base) class name : \
        public base { \
           public: \
           virtual ~name() {}

#define EndInterface };

#define implements public

#endif // INTERFACES_H_
