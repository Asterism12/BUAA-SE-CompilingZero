#include "help.h"
void getHelp() {
	std::string str = R"(
Usage:
  cc0 [options] input [-o file]
or 
  cc0 [-h]
Options:
  -s        ������� c0 Դ���뷭��Ϊ�ı�����ļ�
  -c        ������� c0 Դ���뷭��Ϊ������Ŀ���ļ�
  -h        ��ʾ���ڱ�����ʹ�õİ���
  -o file   �����ָ�����ļ� file

���ṩ�κβ���ʱ��Ĭ��Ϊ -h
�ṩ input ���ṩ -o file ʱ��Ĭ��Ϊ -o out)";
	std::cout << str << std::endl;
}