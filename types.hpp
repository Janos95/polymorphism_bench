//
// Created by jmeny on 21.10.19.
//

#include <array>

#pragma once


using A =  std::array<int, 9>;

struct B {
    B() = default;
    virtual void execute() const = 0;
    virtual ~B() = default;
};


struct X1: B
{
    explicit X1(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[0];}
};

struct X2: B
{
    explicit X2(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[1];}
};

struct X3: B
{
    explicit X3(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[2];}
};

struct X4: B
{
    explicit X4(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[3];}
};

struct X5: B
{
    explicit X5(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[4];}
};

struct X6: B
{
    explicit X6(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[5];}
};

struct X7: B
{
    explicit X7(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[6];}
};

struct X8: B
{
    explicit X8(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[7];}
};

struct X9: B
{
    explicit X9(A& a) : arr(std::addressof(a)) {}
    A* arr;
    void execute() const override {++(*arr)[8];}
};

struct mono
{
    void execute() const{};
};