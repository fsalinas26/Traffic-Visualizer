#pragma once
#include <winsvc.h>
#pragma warning (disable: 4191 4099)
#pragma comment (lib, "cryptlib.lib")
#include "./cryptopp/aes.h"
#include "./cryptopp/modes.h"
#include "./cryptopp/base64.h"
#include "./cryptopp/hex.h"
#include <iosfwd>
#include "cryptopp/stdcpp.h"
#include <algorithm>
#include <winioctl.h>
#include <winternl.h>
#include <iostream>
#include <string>
#include <random>
#include <json.hpp>

using namespace nlohmann;
class c_crypto
{
public:
	static std::string b64_to_binary(const std::string str_in);
	static std::string b64URL_to_binary(const std::string str_in);
	static std::string binary_to_b64url(const std::string str_in);
private:
};
