#pragma once
class Reader
{
public:
	Reader(char* buf, unsigned int buflen) : m_buf(buf), m_buflen(buflen), m_pos(0), outc(nullptr)
	{
	}
	~Reader()
	{
	}

	char* ReadCString()
	{
		while (m_pos < m_buflen && m_buf[m_pos])
		{
			out += m_buf[m_pos];
			m_pos++;
		}

		m_pos++;
		outc = new char[out.size() + 1];
		memcpy(outc, out.c_str(), out.size() + 1);
		return outc;
	}

	unsigned int ReadUint32()
	{
		m_pos += 4;
		return std::static_cast<unsigned int>(m_buf + m_pos - 4);
	}

	unsigned int Tell() const
	{
		return m_pos;
	}

	void Seek(unsigned int pos)
	{
		this->m_pos = pos;
	}

	char* GetBuf() const
	{
		return m_buf + m_pos;
	}

private:
	char* m_buf;
	unsigned int m_buflen;
	unsigned int m_pos;
	std::string out;
	char* outc;

};