cmake_minimum_required(VERSION 3.26)
project(CppEvents)

set(CMAKE_CXX_STANDARD 23)

add_executable(CppEvents main.cpp
        Observer.h
        ObservableConnector.h
        ICloseable.h
        Function.h
        Action.h
        CMethod.h
        CFunctor.h
        MultipleBindingException.h
        MultipleSubscriptionException.h
        NonSubscribedException.h
        NonBoundInvokingException.h
        ObservableInvoker.h)
