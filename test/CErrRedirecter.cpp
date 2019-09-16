#include "CErrRedirecter.h"

#include <cassert>
#include <cstring>  // memset
#include <iostream>

CErrRedirecter::CErrRedirecter()
    : m_locker(m_mutex)
    , m_buffer(nullptr)
    , m_size(0)
    , m_fp(open_memstream(&m_buffer, &m_size))
    , m_old(stderr)
{
    if (!m_fp)
    {
        throw std::runtime_error("open_memstream failed");
    }

    assert(m_old != nullptr);

    stderr = m_fp;
}

CErrRedirecter::~CErrRedirecter()
{
    stderr = m_old;
    std::fclose(m_fp);
    if (m_buffer != nullptr)
    {
        free(m_buffer);
    }
}
