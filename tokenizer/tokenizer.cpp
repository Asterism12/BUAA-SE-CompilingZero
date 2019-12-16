#include "tokenizer.h"
void readAll() {
	for (std::string tp; std::getline(_rdr, tp);)
		_lines_buffer.emplace_back(std::move(tp + "\n"));
	_line = 0;
	return;
}