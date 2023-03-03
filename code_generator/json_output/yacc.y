%{
#include <stdio.h>
#include <stdlib.h>
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "std_common.h"
#include "json.h"
#include "std_list.h"


#define DIR_IN            101
#define DIR_OUT           102
#define DIR_INOUT         103

#define TYPE_STRING       1
#define TYPE_CHAR         2
#define TYPE_UCHAR        3
#define TYPE_INT          4
#define TYPE_UINT         5
#define TYPE_VOID         6
#define TYPE_SHORT        7
#define TYPE_USHORT       8
#define TYPE_STRUCT       10
#define TYPE_SEM          11

#define TYPE_ARRAY_BASE   20
#define TYPE_ARRAY_STRING 21
#define TYPE_ARRAY_CHAR   22
#define TYPE_ARRAY_UCHAR  23
#define TYPE_ARRAY_INT    24
#define TYPE_ARRAY_UINT   25
#define TYPE_ARRAY_VOID   26
#define TYPE_ARRAY_STRUCT 27
#define TYPE_ARRAY_TYPEDEF 29

#define TYPE_PTR_BASE     40
#define TYPE_PTR_STRING   41
#define TYPE_PTR_CHAR     42
#define TYPE_PTR_UCHAR    43
#define TYPE_PTR_INT      44
#define TYPE_PTR_UINT     45
#define TYPE_PTR_VOID     46
#define TYPE_PTR_STRUCT   47
#define TYPE_PTR_HANDLE   48
#define TYPE_PTR_TYPEDEF  49
#define TYPE_PTR_SEM      51

#define TYPE_PTR_PTR_TYPEDEF   52

#define TYPE_64                61
#define TYPE_U64               62
#define TYPE_128               63
#define TYPE_U128              64


#define TYPE_PTR_ARRAY_BASE    80
#define TYPE_PTR_ARRAY_STRING  81
#define TYPE_PTR_ARRAY_CHAR    82
#define TYPE_PTR_ARRAY_UCHAR   83
#define TYPE_PTR_ARRAY_INT     84
#define TYPE_PTR_ARRAY_UINT    85
#define TYPE_PTR_ARRAY_VOID    86
#define TYPE_PTR_ARRAY_STRUCT  87
#define TYPE_PTR_ARRAY_TYPEDEF 89

typedef struct filed_st{
	char name[128];
	int type;
    	char ptr_struct[128];
    	int array_assign;
	std_list_head_t list;
}field_t;

typedef struct struct_st{
	char name[128];
	char typedef_name[128];
	std_list_head_t field_head;
	std_list_head_t list;
}struct_t;

typedef struct arg_st{
	int direction;
	int type;
	char name[128];
    	char ptr_struct[128];
	std_list_head_t list;
}arg_t;

typedef struct ret_st{
	int type;
    	char ptr_struct[128];
}ret_t;

typedef struct function_st{
	std_char_t name[128];
   	char ptr_struct[128];
	std_list_head_t arg_head;
	ret_t ret;
	std_list_head_t list;
}function_t;

typedef struct include_st{
	std_char_t name[128];
	std_list_head_t list;
}include_t;
 
typedef struct define_st{
	std_char_t name[128];
	std_char_t value[128];
	std_list_head_t list;
}define_t;

typedef struct interface_st{
	std_char_t version[32];
	std_char_t module[128];
	std_char_t instance[128];
    	std_list_head_t include_head;
    	std_list_head_t define_head;
	std_list_head_t struct_head;
	std_list_head_t function_head;
}interface_t;

interface_t interface;
 
define_t *def = NULL;
include_t *inc = NULL;
function_t *func = NULL;
struct_t *global_struct = NULL;
struct_t *global_struct_table[128];
int       global_struct_index = 0;
int array_assign = 0;

void new_include()
{
	if (inc == NULL){
		inc = (include_t *)malloc(sizeof(include_t));
		memset(inc, 0, sizeof(include_t));
		STD_INIT_LIST_HEAD(&inc->list);
	}
}

void new_define()
{
	if (def == NULL){
		def = (define_t *)malloc(sizeof(define_t));
		memset(def, 0, sizeof(define_t));
		STD_INIT_LIST_HEAD(&def->list);
	}
}


void new_func()
{
	if (func == NULL){
		func = (function_t *)malloc(sizeof(function_t));
		memset(func, 0, sizeof(function_t));
		STD_INIT_LIST_HEAD(&func->list);
		STD_INIT_LIST_HEAD(&func->arg_head);
	}
}
 
void new_struct()
{
	if (global_struct == NULL){
		printf("NEW_STRUCT\n");
		global_struct = (struct_t *)malloc(sizeof(struct_t));
		memset(global_struct, 0, sizeof(struct_t));
		STD_INIT_LIST_HEAD(&global_struct->list);
		STD_INIT_LIST_HEAD(&global_struct->field_head);
	}
}
 
extern int yylex(void);
void yyerror(char *s);
%}

%union {
        int ival;
        char *str;
}
%token <str> ID STRING
%token <ival> NUM INT FLOAT CHAR DOUBLE VOID UINT UCHAR HANDLE TYPEDEF USHORT SHORT LLONG ULLONG LLLONG ULLLONG SEM DIN DOUT DINOUT

%type  <ival> Type Inout
%type  <str> ArrayUsage Assignment

%token FOR WHILE
%token IF ELSE PRINTF 
%token STRUCT 

%token INCLUDE DEFINE
%token DOT

%right '='
%left AND OR
%left '<' '>' LE GE EQ NE LT GT
%%

start:	block_item_list
	;

block_item_list
	: block_item
	| block_item block_item_list 
	;

block_item
	: Declaration
	| Function 
	;


/* Declaration block */
Declaration: Type Assignment ';' 
        | Type Pointer Assignment ';' 
	| Assignment ';'  	
	| ArrayUsage ';'	
	| Type ArrayUsage ';'   
	| StructStmt ';'
        | Define
	| error	
	;

Define : DEFINE ID STRING  {
	   printf("DEFINE %s %s\n", $2, $3);
	
	   if (!strcmp($2, "VERSION")){
		   snprintf(interface.version, sizeof(interface.version), "%s",  $3+1);
		   strtok(interface.version, "\"");
	   }else if (!strcmp($2, "MODULE")){
		   snprintf(interface.module, sizeof(interface.module), "%s",  $3+1);
		   strtok(interface.module, "\"");
	   }else if (!strcmp($2, "INSTANCE")){
		   snprintf(interface.instance, sizeof(interface.instance), "%s",  $3+1);
		   strtok(interface.instance, "\"");
	   }else{
	     new_define();

	     snprintf(def->name, sizeof(def->name), "%s", $2);
	     snprintf(def->value, sizeof(def->value), "%s", $3);
	     std_list_add(&def->list, &interface.define_head);
	     def = NULL;
	   }
        }
        | DEFINE ID ID{
	     new_define();

	     snprintf(def->name, sizeof(def->name), "%s", $2);
	     snprintf(def->value, sizeof(def->value), "%s", $3);
	     std_list_add(&def->list, &interface.define_head);
	     def = NULL;
        }
        | DEFINE ID NUM{
	     new_define();
	     
	     snprintf(def->name, sizeof(def->name), "%s", $2);
	     snprintf(def->value, sizeof(def->value), "%d", $3);

	     printf("DEFINE ID NUM %s %s\n", def->name, def->value);
	     std_list_add(&def->list, &interface.define_head);
	     def = NULL;
        }
        ;

/* Assignment block */
Assignment: ID '=' Assignment
	|   NUM
	|   ID
	;


/* Array Usage */
ArrayUsage : ID'['NUM']' {$$=$1; array_assign = $3;}
   	|ID'['']' {$$=$1; array_assign = 0;}
	;

/* Function block */
Function: Type ID '(' ArgListOpt ')' ';' {
	    printf("FUNC: %s RETURN: %d\n", $2, $1);
	    new_func();
	    func->ret.type = $1;
	    snprintf(func->name, sizeof(func->name), "%s", $2);
	    std_list_add(&func->list, &interface.function_head);
	    func = NULL; 
	    
        }
        | Type Pointer ID '(' ArgListOpt ')' ';' {
	    printf("PTR_FUNC: %s RETURN PTR: %d\n", $3, $1);
	    new_func();
	    func->ret.type = TYPE_PTR_BASE + $1;
	    snprintf(func->name, sizeof(func->name), "%s", $3);
	    std_list_add(&func->list, &interface.function_head);
	    func = NULL; 
	    
        }
        | ID Pointer ID '(' ArgListOpt ')' ';' {
	    printf("STRUCT PTR_FUNC: %s RETURN PTR: %s\n", $3, $1);
	    new_func();
	    func->ret.type = TYPE_PTR_TYPEDEF;
	    snprintf(func->name, sizeof(func->name), "%s", $3);
	    snprintf(func->ret.ptr_struct, sizeof(func->ptr_struct), "%s", $1);
	    std_list_add(&func->list, &interface.function_head);
	    func = NULL; 
        }
	;

ArgListOpt: ArgList 
	|
	;

ArgList:  ArgList ',' Arg 
        | Arg 
	;

Arg:	Inout Type ID  {
	   	printf("DIRECTION: %d TYPE:%d ID:%s\n", $1, $2, $3);

	   	new_func();
	   	arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
	   	arg->direction = $1;
	   	arg->type = $2;
	   	snprintf(arg->name, sizeof(arg->name), "%s", $3);
	   	std_list_add(&arg->list, &func->arg_head);
        }
        | Inout Type ArrayUsage    {
		printf("DIRECTION: %d TYPE ARRAY FIELD TYPE:%d ID:%s\n", $1, $2, $3);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = $1;
		arg->type = TYPE_ARRAY_BASE + $2;
		snprintf(arg->name, sizeof(arg->name), "%s", $3);
		std_list_add(&arg->list, &func->arg_head);
	}

	| Inout ID ArrayUsage {
		printf("DIRECTION: %d TYPE ARRAY FIELD TYPE:%s ID:%s\n", $1, $2, $3);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		std_list_add(&arg->list, &func->arg_head);
		arg->direction = $1;
		arg->type = TYPE_ARRAY_TYPEDEF;
		snprintf(arg->name, sizeof(arg->name), "%s", $3);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $2);

	}

	| Inout ID Pointer ArrayUsage {
		printf("DIRECTION: %d TYPE PTR ARRAY FIELD TYPE:%s ID:%s\n", $1, $2, $4);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		std_list_add(&arg->list, &func->arg_head);
		arg->direction = $1;
		arg->type = TYPE_PTR_ARRAY_TYPEDEF;
		snprintf(arg->name, sizeof(arg->name), "%s", $4);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $2);
	}
	| Inout Type Pointer ID {
		printf("DIRECTION: %d TYPE_PTR: ID:%s\n", $1, $4);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = $1;
		arg->type = TYPE_PTR_BASE +  $2;
		snprintf(arg->name, sizeof(arg->name), "%s", $4);
		std_list_add(&arg->list, &func->arg_head);
	}
	| Inout STRUCT ID Pointer ID {
		printf("DIRECTION: %d TYPE_PTR_STRUCT:%s name:%s\n", $1, $3, $5);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = $1;
		arg->type = TYPE_PTR_STRUCT;
		snprintf(arg->name, sizeof(arg->name), "%s", $5);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $3);
		std_list_add(&arg->list, &func->arg_head);
	}
	| Inout ID Pointer ID {
		printf("DIRECTION: %d TYPE_PTR_TYPEDEF :%s name:%s\n", $1, $2, $4);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = $1;
		arg->type = TYPE_PTR_TYPEDEF;
		snprintf(arg->name, sizeof(arg->name), "%s", $4);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $2);

		std_list_add(&arg->list, &func->arg_head);
	}
	| Inout ID Pointer Pointer ID {
		printf("DIRECTION: %d TYPE_PTR_PTR_TYPEDEF :%s name:%s\n", $1, $2, $5);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = $1;
		arg->type = TYPE_PTR_PTR_TYPEDEF;
		snprintf(arg->name, sizeof(arg->name), "%s", $5);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $2);

		std_list_add(&arg->list, &func->arg_head);
	}
        | Type ID  {
		printf("DIRECTION: %d TYPE:%d ID:%s\n", 0, $1, $2);

		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = DIR_IN;
		arg->type = $1;
		snprintf(arg->name, sizeof(arg->name), "%s", $2);
		std_list_add(&arg->list, &func->arg_head);
	}

	| Type ArrayUsage    {
		printf("TYPE ARRAY FIELD TYPE:%d ID:%s\n", $1, $2);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = DIR_IN;
		arg->type = TYPE_ARRAY_BASE + $1;
		snprintf(arg->name, sizeof(arg->name), "%s", $2);
		std_list_add(&arg->list, &func->arg_head);
	}

    	| ID ArrayUsage {
		printf("TYPE ARRAY FIELD TYPE:%s ID:%s\n", $1, $2);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		std_list_add(&arg->list, &func->arg_head);
		arg->direction = DIR_IN;
        	arg->type = TYPE_ARRAY_TYPEDEF;
	    	snprintf(arg->name, sizeof(arg->name), "%s", $2);
	    	snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $1);

      	}

    	| ID Pointer ArrayUsage {
		printf("TYPE PTR ARRAY FIELD TYPE:%s ID:%s\n", $1, $3);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		std_list_add(&arg->list, &func->arg_head);
		arg->direction = DIR_IN;
        	arg->type = TYPE_PTR_ARRAY_TYPEDEF;
	    	snprintf(arg->name, sizeof(arg->name), "%s", $3);
	    	snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $1);
      	}
	| Type Pointer ID {
	   	printf("TYPE_PTR: ID:%s\n", $3);
	   	new_func();
	   	arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
	   	arg->direction = DIR_IN;
	   	arg->type = TYPE_PTR_BASE +  $1;
	   	snprintf(arg->name, sizeof(arg->name), "%s", $3);
	   	std_list_add(&arg->list, &func->arg_head);
        }
    | STRUCT ID Pointer ID {
	   	printf("TYPE_PTR_STRUCT:%s name:%s\n", $2, $4);
	   	new_func();
	   	arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
	   	arg->direction = DIR_IN;
	   	arg->type = TYPE_PTR_STRUCT;
	   	snprintf(arg->name, sizeof(arg->name), "%s", $4);
	   	snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $2);
	   	std_list_add(&arg->list, &func->arg_head);
      	}
       	| ID Pointer ID {
	   	printf("TYPE_PTR_TYPEDEF :%s name:%s\n", $1, $3);
		new_func();
	   	arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
	   	arg->direction = DIR_IN;
	   	arg->type = TYPE_PTR_TYPEDEF;
	   	snprintf(arg->name, sizeof(arg->name), "%s", $3);
	   	snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $1);

	   	std_list_add(&arg->list, &func->arg_head);
	}
	| ID Pointer Pointer ID {
		printf("TYPE_PTR_PTR_TYPEDEF :%s name:%s\n", $1, $4);
		new_func();
		arg_t *arg = (arg_t *)calloc(1, sizeof(arg_t));
		arg->direction = DIR_IN;
		arg->type = TYPE_PTR_PTR_TYPEDEF;
		snprintf(arg->name, sizeof(arg->name), "%s", $4);
		snprintf(arg->ptr_struct, sizeof(arg->ptr_struct), "%s", $1);

		std_list_add(&arg->list, &func->arg_head);
	}
	;

Pointer: '*'
         ;




/* Type Identifier block */
Type:	INT 
    	| UINT
	| FLOAT
	| CHAR
    	| UCHAR
	| DOUBLE
    	| SHORT
    	| USHORT
    	| LLONG
    	| ULLONG
	| LLLONG
	| ULLLONG
	| SEM
	| VOID
    	| HANDLE
	;

Inout:	DIN
	| DOUT
	| DINOUT
	;

struct_declaration
        : Type Assignment ';'    {
		new_struct();
		printf("STRUCT FILED TYPE:%d ID:%s\n", $1, $2);
		field_t *field = (field_t *)calloc(1, sizeof(field_t));
		field->type = $1;
		snprintf(field->name, sizeof(field->name), "%s", $2);
		std_list_add(&(field->list), &(global_struct->field_head));
 	}
        | Type Pointer Assignment ';' {
		new_struct();
		printf("STRUCT FILED TYPE_PTR:%d ID:%s\n", (int)$1, $3);

		field_t *field = (field_t *)calloc(1, sizeof(field_t));

		field->type = TYPE_PTR_BASE + $1;
		snprintf(field->name, sizeof(field->name), "%s", $3);
		std_list_add(&field->list, &global_struct->field_head);
 	}
	| ArrayUsage ';'
	| Type ArrayUsage ';'   {
		printf("STRUCT ARRAY FIELD TYPE:%d ID:%s\n", $1, $2);
		new_struct();
		field_t *field = (field_t *)calloc(1, sizeof(field_t));
		field->type = TYPE_ARRAY_BASE + $1;
        	field->array_assign = array_assign;
		snprintf(field->name, sizeof(field->name), "%s", $2);
		std_list_add(&field->list, &global_struct->field_head);
	}
    	| ID ArrayUsage ';' {
		printf("TYPEDEF ARRAY FIELD TYPE:%s ID:%s\n", $1, $2);
		new_struct();
		field_t *field = (field_t *)calloc(1, sizeof(field_t));
        	std_list_add(&field->list, &global_struct->field_head);
        	field->type = TYPE_ARRAY_TYPEDEF;
	    	snprintf(field->name, sizeof(field->name), "%s", $2);
	    	snprintf(field->ptr_struct, sizeof(field->ptr_struct), "%s", $1);
      	}
    	| ID Pointer ArrayUsage ';' {
		printf("TYPEDEF PTR ARRAY FIELD TYPE:%s ID:%s\n", $1, $3);
		new_struct();
		field_t *field = (field_t *)calloc(1, sizeof(field_t));
        	std_list_add(&field->list, &global_struct->field_head);
        	field->type = TYPE_PTR_ARRAY_TYPEDEF;
	    	snprintf(field->name, sizeof(field->name), "%s", $1);
	    	snprintf(field->ptr_struct, sizeof(field->ptr_struct), "%s", $1);
      	}
      	| ID Pointer ID ';' {
		new_struct();
		printf("STRUCT FILED TYPE_PTR_TYPEDEF:%s ID:%s\n", $1, $3);

		field_t *field = (field_t *)calloc(1, sizeof(field_t));

		field->type = TYPE_PTR_TYPEDEF;

		snprintf(field->name, sizeof(field->name), "%s", $3);
		snprintf(field->ptr_struct, sizeof(field->ptr_struct), "%s", $1);
		std_list_add(&field->list, &global_struct->field_head);
	}
    	| STRUCT ID Pointer ID ';' {
		new_struct();
		printf("STRUCT FILED TYPE_STRUCT_PTR:%s ID:%s\n", $2, $4);

		field_t *field = (field_t *)calloc(1, sizeof(field_t));

		field->type = TYPE_PTR_STRUCT;

		snprintf(field->name, sizeof(field->name), "%s", $4);
		snprintf(field->ptr_struct, sizeof(field->ptr_struct), "%s", $2);
		std_list_add(&field->list, &global_struct->field_head);
        }
        | {
        	new_struct();
        	printf("STRUCT FILED None\n");
        }
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

/* Struct Statement */
StructStmt
    : STRUCT ID '{' struct_declaration_list '}'  {
		printf("STRUCT __ADD %s\n", $2);
		snprintf(global_struct->name, sizeof(global_struct->name), "%s",  $2);
		std_list_add(&global_struct->list, &interface.struct_head);
		global_struct_table[global_struct_index++] = global_struct;
		global_struct = NULL;
        }
    | TYPEDEF STRUCT ID '{' struct_declaration_list '}' ID  {
	  	printf("TYPEDEF STRUCT __ADD %s %s\n", $3, $7);
		snprintf(global_struct->name, sizeof(global_struct->name), "%s",  $3);
		snprintf(global_struct->typedef_name, sizeof(global_struct->typedef_name), "%s",  $7);
		std_list_add(&global_struct->list, &interface.struct_head);
		global_struct_table[global_struct_index++] = global_struct;
		global_struct = NULL;
        }
    | TYPEDEF STRUCT ID ID  {
	  	printf("TYPEDEF STRUCT ID ID  %s %s\n", $3, $4);
	  	new_struct();
	  	snprintf(global_struct->name, sizeof(global_struct->name), "%s",  $3);
	  	snprintf(global_struct->typedef_name, sizeof(global_struct->typedef_name), "%s",  $4);
	  	std_list_add(&global_struct->list, &interface.struct_head);
	  	global_struct_table[global_struct_index++] = global_struct;
	  	global_struct = NULL;
        }
	;


%%
#include"lex.yy.c"
#include<ctype.h>
int count=0;




char *type_to_string(int type)
{
	switch(type){
		case DIR_IN:return "IN";
		case DIR_OUT:return "OUT";
		case DIR_INOUT:return "INOUT";
		case TYPE_STRING:return "TYPE_STRING";
		case TYPE_CHAR:return "TYPE_CHAR";
		case TYPE_UCHAR:return "TYPE_UCHAR";
		case TYPE_INT:return "TYPE_INT";
		case TYPE_UINT:return "TYPE_UINT";
		case TYPE_VOID:return "TYPE_VOID";
		case TYPE_SHORT:return "TYPE_SHORT";
		case TYPE_USHORT:return "TYPE_USHORT";
		case TYPE_STRUCT:return "TYPE_STRUCT";
		case TYPE_ARRAY_STRING:return "TYPE_ARRAY_STRING";
		case TYPE_ARRAY_CHAR:return "TYPE_ARRAY_CHAR";
		case TYPE_ARRAY_UCHAR:return "TYPE_ARRAY_UCHAR";
		case TYPE_ARRAY_INT:return "TYPE_ARRAY_INT";
		case TYPE_ARRAY_UINT:return "TYPE_ARRAY_UINT";
		case TYPE_ARRAY_VOID:return "TYPE_ARRAY_VOID";
		case TYPE_ARRAY_STRUCT:return "TYPE_ARRAY_STRUCT";
		case TYPE_ARRAY_TYPEDEF:return "TYPE_ARRAY_TYPEDEF";
		case TYPE_PTR_STRING:return "TYPE_PTR_STRING";
		case TYPE_PTR_CHAR:return "TYPE_PTR_CHAR";
		case TYPE_PTR_UCHAR:return "TYPE_PTR_UCHAR";
		case TYPE_PTR_INT:return "TYPE_PTR_INT";
		case TYPE_PTR_UINT:return "TYPE_PTR_UINT";
		case TYPE_PTR_VOID:return "TYPE_PTR_VOID";
		case TYPE_PTR_STRUCT:return "TYPE_PTR_STRUCT";
		case TYPE_PTR_HANDLE:return "TYPE_PTR_HANDLE";
		case TYPE_PTR_TYPEDEF:return "TYPE_PTR_TYPEDEF";
		case TYPE_PTR_PTR_TYPEDEF:return "TYPE_PTR_PTR_TYPEDEF";
		case TYPE_PTR_SEM:return "TYPE_PTR_SEM";
		case TYPE_64:return "TYPE_64";
		case TYPE_U64:return "TYPE_U64";
		case TYPE_128:return "TYPE_128";
		case TYPE_U128:return "TYPE_U128";
		case TYPE_SEM:return "TYPE_SEM";
		case TYPE_PTR_ARRAY_STRING:return "TYPE_PTR_ARRAY_STRING";
		case TYPE_PTR_ARRAY_CHAR:return "TYPE_PTR_ARRAY_CHAR";
		case TYPE_PTR_ARRAY_UCHAR:return "TYPE_PTR_ARRAY_UCHAR";
		case TYPE_PTR_ARRAY_INT:return "TYPE_PTR_ARRAY_INT";
		case TYPE_PTR_ARRAY_UINT:return "TYPE_PTR_ARRAY_UINT";
		case TYPE_PTR_ARRAY_VOID:return "TYPE_PTR_ARRAY_VOID";
		case TYPE_PTR_ARRAY_STRUCT:return "TYPE_PTR_ARRAY_STRUCT";
		case TYPE_PTR_ARRAY_TYPEDEF:return "TYPE_PTR_ARRAY_TYPEDEF";
		default:
		    return "undefined";
	}
}



void create_json_file()
{
    	std_list_head_t *list;
	define_t *def = NULL;
	struct_t *struc = NULL;
	function_t *func = NULL;
	struct json_object *head_object;
	struct json_object *defines_object = json_object_new_array();
	struct json_object *structs_object = json_object_new_array();
	struct json_object *methods_object = json_object_new_array();


	head_object = json_object_new_object();
	json_object_object_add(head_object, "version", json_object_new_string(interface.version));
	json_object_object_add(head_object, "module", json_object_new_string(interface.module));
	json_object_object_add(head_object, "instance", json_object_new_string(interface.instance));
	json_object_object_add(head_object, "instance", json_object_new_string(interface.instance));
	json_object_object_add(head_object, "defines", defines_object);
	json_object_object_add(head_object, "structs", structs_object);
	json_object_object_add(head_object, "methods", methods_object);

	for (list = interface.define_head.prev; list != &interface.define_head; list=list->prev){
		struct json_object *define_object = json_object_new_object();
		def = std_list_entry(list, define_t, list);
		json_object_array_add(defines_object, define_object);
		json_object_object_add(define_object, "name", json_object_new_string(def->name));
		json_object_object_add(define_object, "value", json_object_new_string(def->value));
	}

    	for (list = interface.struct_head.prev; list != &interface.struct_head; list=list->prev){
		struct json_object *struct_object = json_object_new_object();
		struc = std_list_entry(list, struct_t, list);

		json_object_array_add(structs_object, struct_object);
		json_object_object_add(struct_object, "name", json_object_new_string(struc->name));
		json_object_object_add(struct_object, "typedef_name", json_object_new_string(struc->typedef_name));

		std_list_head_t *list2;
		field_t *field;
		struct json_object *fields_object = json_object_new_array();
		json_object_object_add(struct_object, "fields", fields_object);

	for (list2 = struc->field_head.prev; list2 != &struc->field_head; list2=list2->prev){
		struct json_object *field_object = json_object_new_object();
		field = std_list_entry(list2, field_t, list);

		json_object_array_add(fields_object, field_object);
		json_object_object_add(field_object, "type", json_object_new_string(type_to_string(field->type)));
		json_object_object_add(field_object, "name", json_object_new_string(field->name));
		json_object_object_add(field_object, "ptr_struct", json_object_new_string(field->ptr_struct));
		json_object_object_add(field_object, "array_assign", json_object_new_int(field->array_assign));
        }
    }

    for (list = interface.function_head.prev;
         list != &interface.function_head; list=list->prev){

        func = std_list_entry(list, function_t, list);

        struct json_object *method_object = json_object_new_object();

        json_object_array_add(methods_object, method_object);
        struct json_object *ret_object = json_object_new_object();
        json_object_object_add(method_object, "ret", ret_object);
        json_object_object_add(ret_object, "type", json_object_new_string(type_to_string(func->ret.type)));
        json_object_object_add(ret_object, "ptr_struct", json_object_new_string(func->ret.ptr_struct));

        json_object_object_add(method_object, "name", json_object_new_string(func->name));

        std_list_head_t *list2;
        arg_t *arg;
        struct json_object *args_object = json_object_new_array();
        json_object_object_add(method_object, "args", args_object);

        for (list2 = func->arg_head.prev; list2 != &func->arg_head; list2=list2->prev){
            struct json_object *arg_object = json_object_new_object();
            arg = std_list_entry(list2, arg_t, list);

            json_object_array_add(args_object, arg_object);
            json_object_object_add(arg_object, "direction", json_object_new_string(type_to_string(arg->direction)));
            json_object_object_add(arg_object, "type", json_object_new_string(type_to_string(arg->type)));
            json_object_object_add(arg_object, "name", json_object_new_string(arg->name));
            json_object_object_add(arg_object, "ptr_struct", json_object_new_string(arg->ptr_struct));
        }
    }

    printf("%s\n", json_object_to_json_string(head_object));
    json_object_to_file_ext("target/output.json", head_object, JSON_C_TO_STRING_PRETTY);
    
    json_object_put(head_object);
}




int main(int argc, char *argv[])
{
	memset(&interface, 0, sizeof(interface));
	STD_INIT_LIST_HEAD(&interface.include_head);
	STD_INIT_LIST_HEAD(&interface.define_head);
	STD_INIT_LIST_HEAD(&interface.struct_head);
	STD_INIT_LIST_HEAD(&interface.function_head);
	
	yyin = fopen(argv[1], "r");
	
	if(!yyparse())
		printf("\nParsing complete\n");
	else
		printf("\nParsing failed\n");
	
	fclose(yyin);
	
	create_json_file();

	return 0;
}

void yyerror(char *s) {
	printf("%d : %s %s\n", yylineno, s, yytext );
}         

