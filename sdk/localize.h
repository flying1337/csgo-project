//
// Created by ruppet on 05.01.2020.
//

#ifndef uc-sflying-cm-_LOCALIZE_H
#define uc-sflying-cm-_LOCALIZE_H

class c_localize : public c_appsystem {
public:
    virtual bool					add_file(const char* fileName, const char* pPathID = nullptr, bool bIncludeFallbackSearchPaths = false) = 0;
    virtual void					remove_all() = 0;
    virtual wchar_t*				find(const char* tokenName) = 0;
    virtual const wchar_t*			find_safe(const char* tokenName) = 0;
    virtual int						convert_to_unicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes) = 0;
    virtual int						convert_to_ansi(const wchar_t* unicode, char* ansi, int ansiBufferSize) = 0;
    virtual unsigned long	        find_index(const char* tokenName) = 0;
    virtual void					construct_string(wchar_t* unicodeOuput, int unicodeBufferSizeInBytes, const wchar_t* formatString, int numFormatParameters, ...) = 0;
    virtual const char*				get_name_by_index(unsigned long index) = 0;
    virtual wchar_t*				get_value_by_index(unsigned long index) = 0;
};

#endif //uc-sflying-cm-_LOCALIZE_H
