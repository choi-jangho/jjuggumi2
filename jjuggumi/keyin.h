#pragma once
#ifndef _KEYIN_H_
#define _KEYIN_H_

// �Է� �޴� Ű ����
// Ű�� �߰�/������ ��� N_KEY��,
// keyin.c:is_valid():key_list[]�� �Բ� �����ؾ� ��
#define N_KEY		14  // K_UNDEFINED �� �� Ű ����

// ����: #define ��� enum �������ε� ���� ����
#define K_ARROW		224
#define K_UP		72
#define K_DOWN		80
#define K_LEFT		75
#define K_RIGHT		77
#define K_QUIT		'q'
#define K_UNDEFINED	'\0'
#define K_Z			'z'
#define K_SLASH		'/'
#define K_X			'x'
#define K_DOT		46
#define K_CONFIRM	'c'
#define K_STEAL		'a'
#define K_CONCIL	's'
#define K_SPACE		32

typedef int key_t;

key_t get_key(void);

#endif
