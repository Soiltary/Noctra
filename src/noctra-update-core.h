#ifndef ORIVA_UPDATE_CORE_H
#define ORIVA_UPDATE_CORE_H
#include <wchar.h>
#include <stdint.h>
static int updateVersionParts(const wchar_t*s,unsigned parts[4]){int n=0;if(!s||!*s)return 0;for(int i=0;i<4;i++)parts[i]=0;while(*s){if(n>=4||*s<L'0'||*s>L'9')return 0;unsigned value=0;while(*s>=L'0'&&*s<=L'9'){value=value*10+(unsigned)(*s++-L'0');if(value>65535)return 0;}parts[n++]=value;if(!*s)break;if(*s++!=L'.'||!*s)return 0;}return n>=2;}
static int updateIsNewer(const wchar_t*offered,const wchar_t*current){unsigned a[4],b[4];if(!updateVersionParts(offered,a)||!updateVersionParts(current,b))return 0;for(int i=0;i<4;i++){if(a[i]!=b[i])return a[i]>b[i];}return 0;}
static int updateHex64(const wchar_t*s){if(!s||wcslen(s)!=64)return 0;for(int i=0;i<64;i++)if(!((s[i]>=L'0'&&s[i]<=L'9')||(s[i]>=L'a'&&s[i]<=L'f')||(s[i]>=L'A'&&s[i]<=L'F')))return 0;return 1;}
static int updateSafePath(const wchar_t*s){const wchar_t*prefix=L"/downloads/Noctra-Windows-x64-v";if(!s)return 0;size_t n=wcslen(s),start=wcslen(prefix);if(n<=start+4||n>240||wcsncmp(s,prefix,start)||wcscmp(s+n-4,L".exe"))return 0;size_t len=n-start-4;if(len>=40)return 0;wchar_t version[40];wmemcpy(version,s+start,len);version[len]=0;unsigned parts[4];return updateVersionParts(version,parts);}

#endif
