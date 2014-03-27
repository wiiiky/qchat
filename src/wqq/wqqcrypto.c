#include "wqqcrypto.h"

static void upcase_string(char *str, int len)
{
	int i;
	for (i = 0; i < len; ++i) {
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] + 'A' - 'a';
	}
}

/*
 * 该加密函数来自liblwqq
 */
static char *lwqq_enc_pwd(const char *uin, const char *pwd, const char *vc)
{
	int i;
	int uin_byte_length;
	char buf[128] = { 0 };
	char _uin[16] = { 0 };


	/* Calculate the length of uin (it must be 8?) */
	uin_byte_length = strlen(uin) / 4;

	/**
     * Ok, parse uin from string format.
     * "\x00\x00\x00\x00\x54\xb3\x3c\x53" -> {0,0,0,0,54,b3,3c,53}
     */
	for (i = 0; i < uin_byte_length; i++) {
		char u[5] = { 0 };
		char tmp;
		strncpy(u, uin + i * 4 + 2, 2);

		errno = 0;
		tmp = strtol(u, NULL, 16);
		if (errno) {
			return NULL;
		}
		_uin[i] = tmp;
	}

	/* Equal to "var I=hexchar2bin(md5(M));" */
	lutil_md5_digest((unsigned char *) pwd, strlen(pwd), (char *) buf);

	/* Equal to "var H=md5(I+pt.uin);" */
	memcpy(buf + 16, _uin, uin_byte_length);
	lutil_md5_data((unsigned char *) buf, 16 + uin_byte_length,
				   (char *) buf);

	/* Equal to var G=md5(H+C.verifycode.value.toUpperCase()); */
	snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s", vc);
	upcase_string(buf, strlen(buf));

	lutil_md5_data((unsigned char *) buf, strlen(buf), (char *) buf);
	upcase_string(buf, strlen(buf));

	/* OK, seems like every is OK */
	return g_strdup(buf);
}

//#include "md5.h"

/*
 * description 生成webqq登录时用到的密文
 * 				加密方式:将uin转化为数字数组,
 * 				比如"\x00\x00\x00\x00\x54\xb3\x3c\x53"
 * 				被转化为{0,0,0,0,0x54,0xb3,0x3c,0x53}
 * 				将明文md5加密为二进制数后在其后面加上uin的数字数组
 * 				再进行md5加密并转化为十六进制的可读形式
 * 				再将结果后加上验证码进行md5加密并转化为十六进制的可读形式
 * 				这就是最后的结果
 * 				注意:十六进制形式字符串均使用大写表示
 *
 * @param uin检查验证码时返回的结果
 * 		  如:ptui_checkVC('0','!LAY','\x00\x00\x00\x00\x22\x04\x8a\xb0');
 * 		  中的第三个字段
 * @param plain明文密码
 * @verify_code 验证码,如果没有验证码,则使用上面的!LAY,如果有验证码使用该验证码
 *
 * @return 返回加密结果,动态分配内存;失败返回NULL
 */


gchar *wqq_login_encrypt(const gchar * uin, const gchar * plain,
						 const gchar * verify_code)
{
	if (uin == NULL || plain == NULL || verify_code == NULL)
		return NULL;

	return lwqq_enc_pwd(uin, plain, verify_code);
}


/* 
 * hash加密,用于获取好友列表,返回静态内存 
 * 这个实现是我直接从js代码翻译过来的，
 * XXX
 * 这个加密函数也是webqq协议中经常改变的部分!!!!!!!!!!!!!!!!!!!!!!!
 */
const gchar *getHash(guint64 uin, const gchar * ptwebqq)
{
	/*ptwebqq="4e047d86bc1346fe20a97907d7fce2393dd501605c940ab97b397de7d11c1389";*/
	guint a[4] = { 0, 0, 0, 0 };
	gint s;
	gint slen = strlen(ptwebqq);
	guint d[4];
	gchar *j[] = { "EC", "OK" };
	for (s = 0; s < slen; s++) {
		a[s % 4] ^= ptwebqq[s];
	}
	d[0] = ((uin >> 24) & 255) ^ j[0][0];
	d[1] = ((uin >> 16) & 255) ^ j[0][1];
	d[2] = ((uin >> 8) & 255) ^ j[1][0];
	d[3] = ((uin) & 255) ^ j[1][1];
	gchar J[8];
	for (s = 0; s < 8; s++) {
		J[s] = s % 2 == 0 ? a[s >> 1] : d[s >> 1];
	}
	gchar C[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
		'B', 'C', 'D', 'E', 'F'
	};

	static gchar hash[17];
	for (s = 0; s < 8; s++) {
		hash[s * 2] = C[J[s] >> 4 & 15];
		hash[s * 2 + 1] = C[J[s] & 15];
	}
	hash[16] = '\0';
	return hash;
}
