#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <memory>

using std::tuple;
using std::vector;
using std::string;
using std::unique_ptr;

#include "color/color_types.h"
#include "io/kb_input.h"
#include "io/mouse_input.h"

struct Cell
{
    string value; // must be a string for multibyte chars
    Color fg;
    Color bg;
    bool locked; // locked when a double wide is placed before it
    int width;
};

class Frame;

class Framer
{
public:
    Framer();
    virtual ~Framer();
    bool add(Frame* pf, size_t index = static_cast<size_t>(-1));

    bool open();
    void close();
    void invalidate(Frame* pf);
    void invalidate(Frame* pf, int x, int y);

    // virtuals
    virtual void setInitialDepth(EDepthType depth);
    virtual EDepthType getInitialDepth() const;
    virtual void clear() {}

    // pure virtuals
    virtual bool update() = 0;
    virtual tuple<int, int> maxResolution() const = 0;
    virtual void write(Frame* pf, int x, int y, const Cell& cell) = 0;
    void push(unique_ptr<MouseEvent> pme) = 0;
    void push(unique_ptr<KeyboardEvent> pke) = 0;
protected:
    virtual bool onOpen() { return true; }
    virtual void onClose() { }
    virtual bool onAdd(Frame* pf) { return true; }
    vector<Frame*> _frames;
private:
    EDepthType _initial_depth;
};

class FrameGpu
{
public:
    virtual void winched(int width, int height) = 0;
    virtual void unbind() = 0;
};

class Frame
{
public:
    Frame();
    virtual ~Frame();
    void framer(Framer* pfr);
    Framer* framer() const;

    bool scrolling() const;
    void scrolling(bool enable);

    void winched(int width, int height);
    void set_gpu(FrameGpu* gpu);

    bool write(int x, int y, const Cell& cell);

    virtual void push(unique_ptr<KeyEvent> pke) {}
    virtual void push(unique_ptr<MouseEvent> pme) {}
private:
    Framer* _framer;
    FrameGpu* _gpu;
    bool _scrolling;
};

namespace Factory
{
    Framer* create_framer(const string& framerTypeName);
};

