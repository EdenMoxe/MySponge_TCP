#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):_buffer({}),_capacity(capacity),_writecnt(0),_readcnt(0),_endinput(false),_error(false){}

size_t ByteStream::write(const string &data) {
	size_t size=data.size()>(_capacity-_buffer.size())?(_capacity-_buffer.size()):data.size();
	for(size_t i = 0; i < size ; i++){
		_buffer.push_back(data[i]);
		_writecnt++;
	}
#if 0 
	if(_writecnt!=size){
		_buffer.clear();
		_writecnt = 0; 
		return 0;
	}
#endif
	return size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
	string peek_res;
	size_t size=len < _buffer.size()?len:_buffer.size();
	peek_res.assign(_buffer.begin(),_buffer.begin()+size); //左闭右开
    if(peek_res.size()==size)
		return peek_res;
	return {};

}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
	size_t size=len < _buffer.size()?len:_buffer.size();
	for(size_t i =0 ; i < size;i++){
		_buffer.pop_front();
		_readcnt++;
	}	
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
	string read_res;
	size_t size=len < _buffer.size()?len:_buffer.size();
	read_res.assign(_buffer.begin(),_buffer.begin()+size);
	_readcnt+=size;
	_buffer.erase(_buffer.begin(),_buffer.begin()+size);
	if(read_res.size()==size)
		return read_res;
	_buffer.clear();
	_readcnt=0;
    return {};
}

void ByteStream::end_input() {_endinput = true;}

bool ByteStream::input_ended() const {return _endinput;}

size_t ByteStream::buffer_size() const { return _buffer.size();}

bool ByteStream::buffer_empty() const { return _buffer.size()==0;}

bool ByteStream::eof() const { return buffer_empty()&&input_ended();}

size_t ByteStream::bytes_written() const { return _writecnt;}

size_t ByteStream::bytes_read() const { return _readcnt;}

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer.size();}
