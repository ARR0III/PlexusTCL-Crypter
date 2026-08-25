#ifndef PTCL_LICENSE
#define PTCL_LICENSE

#define PTCL_GITHUB_REP_ADDR "ARR0III <https://github.com/ARR0III>"

#ifdef PTCL_RUSSIAN_LANGUAGE
const wchar_t PTCL_LICENSE_INFORMATION[] =
  L"Настоящее программное обеспечение создано для\n"
  L"шифрования файлов размером до 4 ЭиБ, и распространяется\n"
  L"свободно и бесплатно под лицензией GNU GPL версии 3\n"
  L"от 29 июня 2007 года, без каких-либо гарантий.";
const wchar_t PTCL_AUTHORS_INFORMATION[] =
  L"Разработка и дизайн: "   PTCL_GITHUB_REP_ADDR;
#else
const wchar_t PTCL_LICENSE_INFORMATION[] =
  L"This software is designed to encrypt files up to 4 EiB in size, and\n"
  L"is distributed freely and without charge under the GNU GPL\n"
  L"version 3 license of June 29, 2007, without any warranties.";
const wchar_t PTCL_AUTHORS_INFORMATION[] =
  L"Development and design: " PTCL_GITHUB_REP_ADDR;
#endif

#endif
