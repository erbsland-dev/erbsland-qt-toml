cmake_minimum_required(VERSION 3.25)


target_sources(erbsland-qt-toml PRIVATE
        CharReader.hpp
        CharReader.cpp
        DataInputStream.hpp
        DataInputStream.cpp
        FileInputStream.hpp
        FileInputStream.cpp
        NumberSystem.hpp
        StreamState.hpp
        StringInputStream.hpp
        StringInputStream.cpp
        TextStreamInputStream.hpp
        TextStreamInputStream.cpp
        Token.hpp
        Token.cpp
        TokenType.hpp
        TokenType.cpp
        Tokenizer.hpp
        Tokenizer.cpp
        ParserData.hpp
        ParserData.cpp
)

