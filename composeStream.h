// ComposeStream
// Ties two std::ostream together.
// Copied from https://stackoverflow.com/a/1761027/58456
//

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

class ComposeStream : public std::ostream {
    struct ComposeBuffer : public std::streambuf {
        void addBuffer(std::streambuf* buf)
        {
            bufs.push_back(buf);
        }
        virtual int overflow(int c)
        {
            std::for_each(bufs.begin(), bufs.end(), std::bind2nd(std::mem_fun(&std::streambuf::sputc), c));
            return c;
        }

    private:
        std::vector<std::streambuf*> bufs;
    };
    ComposeBuffer myBuffer;

public:
    ComposeStream()
        : std::ostream(NULL)
    {
        std::ostream::rdbuf(&myBuffer);
    }
    void linkStream(std::ostream& out)
    {
        out.flush();
        myBuffer.addBuffer(out.rdbuf());
    }
};