#include <Windows.h>
#include "crypto.h"
#include "xor.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "sha.h"

using namespace CryptoPP;
std::string c_crypto::b64_to_binary(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64Decoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}
std::string c_crypto::binary_to_b64url(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64URLEncoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}
std::string c_crypto::b64URL_to_binary(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64URLDecoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}
static std::string b64URL_to_binaryString(std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64URLDecoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}


