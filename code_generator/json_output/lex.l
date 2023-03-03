L	  [a-zA-Z_]
alpha [a-zA-Z_]
digit [0-9]

%%
[ \t]		;
[ \n]   { yylineno = yylineno + 1;}
int	{yylval.ival = 4; return INT;}
float   {yylval.ival = 22; return FLOAT;}
char    {yylval.ival = 2; return CHAR;}
void    {yylval.ival = 6; return VOID;}
double  {yylval.ival = 21; return DOUBLE;}
std_bool_t    {yylval.ival = 4; return INT;}
std_int_t    {yylval.ival = 4; return INT;}
std_uint_t   {yylval.ival = 5; return UINT;}
std_char_t   {yylval.ival = 2; return CHAR;}
std_uchar_t  {yylval.ival = 3; return UCHAR;}
std_void_t   {yylval.ival = 6; return VOID;}
std_rv_t     {yylval.ival = 4; return INT;}
std_8_t      {yylval.ival = 2; return CHAR;}
std_u8_t     {yylval.ival = 3; return UCHAR;}
std_16_t     {yylval.ival = 7; return SHORT;}
std_u16_t    {yylval.ival = 8; return USHORT;}
std_32_t     {yylval.ival = 4; return INT;}
std_u32_t    {yylval.ival = 5; return UINT;}
std_sem_t    {yylval.ival = 11; return SEM;}
std_64_t     {yylval.ival = 61; return LLONG;}
std_u64_t    {yylval.ival = 62; return ULLONG;}
std_128_t    {yylval.ival = 63; return LLLONG;}
std_u128_t   {yylval.ival = 64; return ULLLONG;}


IN	         {yylval.ival = 101; return DIN;}
OUT	         {yylval.ival = 102; return DOUT;}
INOUT	     {yylval.ival = 103; return DINOUT;}

handle_t     {yylval.ival = 8; return HANDLE;}
for 	return FOR;
while	return WHILE;
if	return IF;
else	return ELSE;
printf  return PRINTF;
struct 	return STRUCT;
typedef return TYPEDEF;
#define  return DEFINE;


L?\"(\\.|[^\\"\n])*\"	{ yylval.str = strdup((char *)yytext); return STRING; }
^"#include ".+ ;
{digit}+       {yylval.ival = atoi(strdup((char *)yytext)); return NUM;}
{alpha}({alpha}|{digit})* {yylval.str = strdup((char *)yytext); return ID;}
"<="    return LE;
">="    return GE;
"=="    return EQ;
"!="    return NE;
">"	    return GT;
"<"	    return LT;
"."     return DOT;
\/\/.* ;
\/\*(.*\n)*.*\*\/ ;
.       return yytext[0];
%%
