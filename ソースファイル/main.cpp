/////////////////////////////////////////////////////////////////////////////
//	���C������ (main.cpp)
//	Author : TAKUMI UENO
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
//	�C���N���[�h�t�@�C��
//--------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CScreen.h"

//--------------------------------------------------------------------------
//	�}�N����`
//--------------------------------------------------------------------------
#define MAX_NAME (10)			//�L�����N�^�[���̍ő啶����
#define MAX_PLAYER (2)			//�v���C���[�̍ő吔

//--------------------------------------------------------------------------
//	�\���̒�`
//--------------------------------------------------------------------------
typedef struct
{
	char Name[MAX_NAME];		//�L�����N�^�[�̖��O
	int Life;					//�L�����N�^�[��HP
	int Attack;					//�L�����N�^�[�̍U����
	int Def;					//�L�����N�^�[�̖h���
	int Speed;					//�L�����N�^�[�̂��΂₳
}PLAYER;

enum SELECT
{
	CREATE,
	BATTLE,
	MAKE,
	END
};

//--------------------------------------------------------------------------
//�@�v���g�^�C�v�錾
//--------------------------------------------------------------------------
int create(PLAYER *player, int root);
enum SELECT title ();
int calculation(PLAYER *player, int CNTname);
int makeSta (PLAYER *player);
int save (PLAYER *player);
int load(PLAYER *player);
int battleAnime (PLAYER *player);
bool battle ( PLAYER *ATCplayer, PLAYER *DEFplayer );

////////////////////////////////////////////////////////////////////////////
// �֐���	: ���C���֐�
// �Ԃ�l	: 0
// ����		: ���C������
////////////////////////////////////////////////////////////////////////////
int  main( void )
{
	//�֐��錾
	PLAYER player[7];			//�Z�[�u�f�[�^�̃L�����N�^�[
	enum SELECT select;			//��������
	int CNTname = 0;			//���O�̕�����
	bool bEndBatttle = false;	//�u�[���^
	int nTurn = 0;				//�^�[�����̍U��
	int check;					//�Z�[�u���邩�ۂ�
	int result;					//�퓬����
	int titleBGM;				//title��BGM
	int makeBGM;				//make����BGM
	int battleBGM;				//battle����BGM
	int winBGM;					//��������BGM
	int loseBGM;				//�s�k����BGM
	int root;					//�G����邩�v���C���[����邩

	//-------------------------------
	// BGM�n���h���w��
	//-------------------------------
	/*titleBGM = OPENMP3("�����Ƀ^�C�g����ʂŗ�������MP3�f�[�^���g���q�܂ŏ����ĉ�����");
	if(titleBGM == 0)
	{
		CLS();
		printf("�yerror�z�^�C�g��BGM���J���܂���ł����B\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	makeBGM = OPENMP3("�����Ƀ��C�L���O��ʂŗ�������MP3�f�[�^���g���q�܂ŏ����ĉ�����");
	if ( makeBGM ==0)
	{
		printf("�yerror�z���C�L���OBGM���J���܂���ł����B\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	battleBGM = OPENMP3("�����Ƀo�g����ʂŗ�������MP3�f�[�^���g���q�܂ŏ����ĉ�����");
	if ( battleBGM ==0)
	{
		printf("�yerror�z�o�g��BGM���J���܂���ł����B\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	winBGM = OPENMP3("�����ɏ������ɗ�������MP3�f�[�^���g���q�܂ŏ����ĉ�����");
	if ( battleBGM ==0)
	{
		printf("�yerror�z����BGM���J���܂���ł����B\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	loseBGM = OPENMP3("�����ɔs�k���ɗ�������MP3�f�[�^���g���q�܂ŏ����ĉ�����");
	if ( battleBGM ==0)
	{
		printf("�yerror�z�s�kBGM���J���܂���ł����B\n");
		rewind(stdin);
		getchar();
		exit(0);
	}*/

	//-------------------------------
	//�^�C�g�����
	//-------------------------------
	CUROFF();
	while(1)
	{
		PLAYMP3(titleBGM, 1);
		select = title();
		//select = BATTLE;

		//-------------------------------
		// ����I��
		//-------------------------------
		switch(select)
		{
		case CREATE:
			{
				root = 0;
				STOPMP3(titleBGM);
				PLAYMP3(makeBGM, 1);

				CNTname = create(&player[0], root);
				calculation(&player[0], CNTname);
				check = makeSta(&player[0]);
				if(check == 0)
				{
					save(&player[0]);
					STOPMP3(makeBGM);
				}
				else
				{
					STOPMP3(makeBGM);
					break;
				}
				break;
			}

		case BATTLE:
			{
				root = 1;

				if( load(&player[0]) == 0 )
				{
					STOPMP3(titleBGM);
					break;
				}
				CNTname = create(&player[1], root);
				calculation(&player[1], CNTname);
				//makeSta(&player[1]);

				STOPMP3(titleBGM);
				PLAYMP3(battleBGM, 1);

				result = battleAnime(&player[0]);

				STOPMP3(battleBGM);

				if(result == 0)
				{
					PLAYMP3(loseBGM, 0);

					//�N���b�N��Enter�҂�
					INPCLEAR();
					while(1)
					{
						if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
						{
							WAIT(50);
							break;
						}
					}
					STOPMP3(loseBGM);
				}
				else
				{
					PLAYMP3(winBGM, 0);

					//�N���b�N��Enter�҂�
					INPCLEAR();
					while(1)
					{
						if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
						{
							WAIT(50);
							break;
						}
					}
					STOPMP3(winBGM);
				}
				break;
			}

		case END:
			{
				CLOSEMP3(makeBGM);
				return 0;
				break;
			}

		default:
			{
				CLS();
				printf("�\�z�O�̃G���[");
				rewind(stdin);
				getchar();
				exit(0);
			}
		}
	}

	/*input(&player[0]);
	input(&player[1]);
	
	//-------------------------------
	//�o�g������
	//-------------------------------
	
	if(player->nSpeed < (player+1)->nSpeed)
	{
		nTurn = 1;
	}
	while(bEndBatttle == false)      //�|���܂�
	{
		Output(&player[0]);
		bEndBatttle = Battle(&player[nTurn] , &player[nTurn^1]);
		nTurn ^= 1;
	}*/


	// �L�[�{�^�������҂��̏��� 
	rewind( stdin );
	getchar();

	// �I��
	exit(0);
}

////////////////////////////////////////////////////////////////////////////
// �֐���	: title
// �Ԃ�l	: CREATE, BATTLE, END
// ����		: �^�C�g����ʍ쐬
////////////////////////////////////////////////////////////////////////////
enum SELECT title ()
{
	//�֐��錾
	int x,y;				//�^�C�g����x���Ay��
	int count;				//�^�C�g���̃X�y�[�X
	int loop = 0;			//�I�����ڂ̃��[�v
	char title[][25][89] =	//�^�C�g��
	{
		{
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
			"][:::::::::::::::::::::::::::::::::::::::::::: .:  ..    ..    ..    ..    ::][",
			"[]:::::::::::::::::::::::::::::::::::::::::::: :: ::::  ::::  ::::  ::::     []",
			"][::::::::::::::::::::::::::::::::::::::::::   ;  ::::  ::::  ::::  ::::     ][",
			"[]:::::::::::::::::::::::::::::::::::::::::                                  []",
			"][::::::::::::::::::::::::::::::   :::::;;   ..    ..    ..    ..    ..      ][",
			"[]::::::::::::::::::::::::::::     :::::    ::::  ::::  ::::  ::::  ::::     []",
			"][::::::::::::::::::::::::         :::::    ::::  ::::  ::::  ::::  ::::     ][",
			"[]:::::::::::::::::::::                                                      []",
			"][::::::::::::::::::       ..    ..    ..    ..    ..    ..    ..    ..      ][",
			"[]::::::::::::::::::::    ::::  ::::  ::::  ::::  ::::  ::::  ::::  ::::     []",
			"][::::::::::::::::  ;     ::::  ::::  ::::  ::::  ::::  ::::  ::::  ::::     ][",
			"[]:::::::::::::::                   Copyright(C) 00000000 all right reserved []",
		},

		{
						"[]                                              []",
						"[]      ##                                      []",
						"][  ##########                        ##        ][",
						"[]        ##                         ##         []",
						"][      ####                        ##          ][",
						"[]    ####  ##    ############     ##           []",
						"][  ##  ##    ##                  ##      ##    ][",
						"[]      ##                       ############   []",
						"][      ##                                  ##  ][",
						"[]                                              []",
		},

		{
			"[]�P�P�P�P�P�P�P�P�P|",
			"][      CREATE      |",
			"[]__________________|"
		},

		{
			"[]�P�P�P�P�P�P�P�P�P|",
			"][      BATTLE      |",
			"[]__________________|"
		},

		{
			"[]�P�P�P�P�P�P�P�P�P|",
			"][      END         |",
			"[]__________________|"
		},
	};

	//-------------------------------
	// �^�C�g����ʏo��
	//-------------------------------
	CLS(WHITE, BLACK);
	for(count = 25; count > 0; count--)
	{	
		for(y = 0; y < 26 - count; y++)
		{
			LOCATE(1, count + y);
			for(x = 0; x < 79; x++)
			{
				printf("%c",title[0][y][x]);
			}
		}
		WAIT(50);
	}

	WAIT(300);

	//-------------------------------
	// �I�����ڏo��
	//-------------------------------
	for(count = 21; count > 0; count--)
	{
		for(y = 0; y < 3; y++)
		{
			LOCATE(1, 12 + y);
			for(x = 0; x < 21 - count; x++)
			{
				printf("%c",title[2][y][count + x]);
			}
		}

		for(y = 0; y < 3; y++)
		{
			LOCATE(1, 16 + y);
			for(x = 0; x < 21 - count; x++)
			{
				printf("%c",title[3][y][count + x]);
			}
		}

		for(y = 0; y < 3; y++)
		{
			LOCATE(1, 20 + y);
			for(x = 0; x < 21 - count; x++)
			{
				printf("%c",title[4][y][count + x]);
			}
		}
		WAIT(80);
	}

	for(count = 1; count < 13; count++)
	{

		for(y = 0; y < 3; y++)
		{
			LOCATE(count, 12 + y);
			for(x = 0; x < 21; x++)
			{
				printf("%c",title[2][y][x]);
			}
			LOCATE(count - 1, 12 + y);
			printf("%c",title[0][11 + y][count - 2]);
		}

		for(y = 0; y < 3; y++)
		{
			LOCATE(count, 16 + y);
			for(x = 0; x < 21; x++)
			{
				printf("%c",title[3][y][x]);
			}
			LOCATE(count - 1, 16 + y);
			printf("%c",title[0][15 + y][count - 2]);
		}

		for(y = 0; y < 3; y++)
		{
			LOCATE(count, 20 + y);
			for(x = 0; x < 21; x++)
			{
				printf("%c",title[4][y][x]);
			}
			LOCATE(count - 1, 20 + y);
			printf("%c",title[0][19 + y][count - 2]);
		}

		WAIT(80);
	}

	//-------------------------------
	// �^�C�g�����S�o��
	//-------------------------------
	for(count = 10; count >= 0; count--)
	{
		for(y = 0; y < 10 - count; y++)
		{
			LOCATE(15, y + 1);
			for(x = 0; x < 50; x++)
			{
				printf("%c",title[1][count + y][x]);
			}
			printf("\n");
		}
		WAIT(80);
	}

	//-------------------------------
	// �I�����ꂽ�Ƃ��̕\���ؑ�
	//-------------------------------
	INPCLEAR();
	while(1)
	{
		//�L�[�{�[�h����
		if (INP(PK_UP) != 0)
		{
			loop = (loop + 2) % 3;
			WAIT(50);
		}
		if (INP(PK_DOWN) != 0)
		{
			loop = (loop + 1) % 3;
			WAIT(50);
		}

		//CREATE
		if(loop == 0
			||(INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 15 && INP(PM_CURY) > 11))
		{
			if( (INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 15 && INP(PM_CURY) > 11) )
			{
				loop = 5;
			}

			COLOR(BLACK, CORAL);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 12 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[2][y][x]);
				}
			}
			LOCATE(19 , 13);
			printf(">");

			if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
			{
				return CREATE;
			}
		}
		else
		{
			COLOR(WHITE, BLACK);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 12 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[2][y][x]);
				}
			}
		}

		//BATTLE
		if(loop == 1
			||(INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 19 && INP(PM_CURY) > 15))
		{
			if((INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 19 && INP(PM_CURY) > 15))
			{
				loop = 5;
			}

			COLOR(BLACK, CORAL);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 16 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[3][y][x]);
				}
			}
			LOCATE(19 , 17);
			printf(">");

			if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
			{
				return BATTLE;
			}
		}
		else
		{
			COLOR(WHITE, BLACK);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 16 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[3][y][x]);
				}
			}
		}

		//END
		if(loop == 2
			||(INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 23 && INP(PM_CURY) > 19))
		{
			if((INP(PM_CURX) < 33 && INP(PM_CURX) > 11
			&& INP(PM_CURY) < 23 && INP(PM_CURY) > 19))
			{
				loop = 5;
			}

			COLOR(BLACK, CORAL);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 20 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[4][y][x]);
				}
			}
			LOCATE(19 , 21);
			printf(">");

			if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
			{
				return END;
			}
		}
		else
		{
			COLOR(WHITE, BLACK);
			for(y = 0; y < 3; y++)
			{
				LOCATE(12, 20 + y);
				for(x = 0; x < 21; x++)
				{
					printf("%c",title[4][y][x]);
				}
			}
		}
		WAIT(50);
	}
	CLS();
}

////////////////////////////////////////////////////////////////////////////
// �֐���	: create
// �Ԃ�l	: 0
// ����		: �L�����N�^�[�̃X�e�[�^�X�쐬
////////////////////////////////////////////////////////////////////////////
int create(PLAYER *player, int root)
{
	//�֐��錾
	int x,y;			//AA�̎�
	int check;			//�������`�F�b�N
	char display[25][95] =
	{
		"][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::[]                              []::::::::::::::::::::[]",
		"][::::::::::::::::::::::[]         (10�����ȓ�)         []::::::::::::::::::::][",
		"[]::::::::::::::::::::::[]    >                         []::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"][::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[]",
		"[]::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::][",
		"][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][",
	};

	//-------------------------------
	// ���O����
	//-------------------------------
	CLS(WHITE, BLACK);
	for(y = 0; y < 25; y++)
	{
		LOCATE(1, y + 1);
		for(x = 0; x < 79; x++)
		{
			printf("%c",display[y][x]);
		}
	}
	
	if(root == 0)
	{
		LOCATE(33, 10);
		printf("���O����͂��Ă��������B");
	}
	else
	{
		LOCATE(27, 10);
		printf("����̖��O����͂��Ă��������B");
	}

	while(1)
	{
		CURON();
		LOCATE(32, 12);
		scanf("%s",player->Name);
		check = strlen(player->Name);

		if(check > 10)
		{
			for(y = 0; y < 25; y++)
			{
				LOCATE(1, y + 1);
				for(x = 0; x < 79; x++)
				{
					printf("%c",display[y][x]);
				}
			}
			LOCATE(34, 10);
			printf("���O���������܂��B");
		}
		else
		{
			return check;
		}
	}

	rewind(stdin);
	getchar();
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : calculation
// �Ԃ�l : 0
// ����   : �X�e�[�^�X�v�Z
////////////////////////////////////////////////////////////////////////////
int calculation (PLAYER *player, int CNTname)
{
	//�֐��錾
	int data = 0;			//�L�����N�^�[���𐔒l�������f�[�^
	int loop;				//for������

	//-------------------------------
	// �̗͌v�Z
	//-------------------------------
	CLS();
	for(loop = 1; loop <= CNTname; loop++)
	{
		data += ( (int)player->Name[loop - 1] * 123 ) * loop;
	}

	data %= 1000;

	if(data < 0)
	{
		data *= -1;
	}
	if(data < 100)
	{
		data += 100;
	}
	
	player->Life = data;

	//-------------------------------
	// �U���͌v�Z
	//-------------------------------
	data = 0;
	for(loop = 1; loop <= CNTname; loop++)
	{
		data += ( player->Life - (int)player->Name[loop - 1] ) + CNTname;
	}

	data %= 300;

	if(data < 0)
	{
		data *= -1;
	}
	if(data < 50)
	{
		data += 30;
	}
	
	player->Attack = data;

	//-------------------------------
	// �h���
	//-------------------------------
	data = 0;
	for(loop = 1; loop <= CNTname; loop++)
	{
		data +=  ( player->Attack + player->Name[loop - 1] ) % 500;
	}

	data %= 300;

	if(data < 0)
	{
		data *= -1;
	}
	if(data < 50)
	{
		data += 30;
	}
	
	player->Def = data;

	//-------------------------------
	// ���΂₳
	//-------------------------------
	data = 0;
	for(loop = 1; loop <= CNTname; loop++)
	{
		data += ( player->Name[loop - 1] + player->Life + player->Attack + player->Def ) / CNTname;
	}

	data %= 300;

	if(data < 0)
	{
		data *= -1;
	}
	if(data < 50)
	{
		data += 30;
	}
	
	player->Speed = data;

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : makeSta
// �Ԃ�l : 0, 1
// ����   : �L�����N�^�[�̃X�e�[�^�X����
////////////////////////////////////////////////////////////////////////////
int makeSta (PLAYER *player)
{
	//�֐��錾
	int loop;				//for������
	int key = 0;			//�L�[�{�[�h����
	int x, y;				//AA�̏c��
	char display[25][102] =
	{
		
			"][][][][][][][][][][][][][][][][][][][][][][][][][][::::][][                ][][",
			"[]             ][:::::::::::::::::::::[][][][][][][]::::[][][STATS]         [][]",
			"][  NAME:      []::::::::::::::::::::::[][][][][][][::::][][�̗�     :      ][][",
			"[]             ][:::::::::::::::::::::::[][][][][][]::::[][]::::::::::::::::[][]",
			"][             []::::::::::::::::::::::::[][][][][][::::][][�U��     :      ][][",
			"[]             ] �^              �_:::::::[][][][][]::::[][]::::::::::::::::[][]",
			"][][][]][][][][]|                 `::::::::[][][][][::::][][�h��     :      ][][",
			"[][][]::::::::::|     []    []     |::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::|         -        |::::::::][][][][::::][][���΂₳ :      ][][",
			"[][][]::::::::::|                  |::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::::                ::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]:::::::::::::::.         .::::::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][:::::::::::::�^         �_�_::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]:::::::::::��.:|        |:. ��::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::::::|          |::::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]::::::::::::::|          |::::::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::::::::| :::: |::::::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]:::::::::::::::::| :: |:::::::::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::       =    =       ::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]::::::::::::::::::::::::::::::::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][                                                                            ][",
			"[]                                                                            []",
			"][                                                                            ][",
			"[]                                                                            []",
		
	};
	
	//-------------------------------
	// �X�e�[�^�X���
	//-------------------------------
	for(y = 0; y < 25; y++)
	{
		LOCATE(1, y + 1);
		for(x = 0; x < 79; x++)
		{
			printf("%c",display[y][x]);
		}
	}

	LOCATE(4, 5);
	printf("%s",player->Name);
	LOCATE(72, 3);
	printf("%d",player->Life);
	LOCATE(72, 5);
	printf("%d",player->Attack);
	LOCATE(72, 7);
	printf("%d",player->Def);
	LOCATE(72, 9);
	printf("%d",player->Speed);
	LOCATE(24, 21);
	printf("���̃L�����N�^�[���Z�[�u���܂����H");

	INPCLEAR();
	loop = 0;
	while(1)
	{
		if( INP(PK_RIGHT) != 0 || INP(PK_LEFT) != 0 )
		{
			key = ( key + 1 ) % 2;
			WAIT(100);
		}

		if( key == 0
			|| INP(PM_CURX) >= 26 && INP(PM_CURX) <= 35
			&& INP(PM_CURY) == 24 )
		{
			if( INP(PM_CURX) >= 26 && INP(PM_CURX) <= 35
			&& INP(PM_CURY) == 24 )
			{
				key = 3;
			}

			COLOR(BLACK, CORAL);
			LOCATE(26, 24);
			printf("[] YES []");

			if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && loop == 0 )
			{
				return 0;
			}
			if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && loop == 1 )
			{
				return 1;
			}
		}
		else
		{
			COLOR(WHITE, BLACK);
			LOCATE(26, 24);
			printf("[] YES []");
		}

		if( key == 1
			|| INP(PM_CURX) >= 49 && INP(PM_CURX) <= 57
			&& INP(PM_CURY) == 24 )
		{
			if( INP(PM_CURX) >= 49 && INP(PM_CURX) <= 57
			&& INP(PM_CURY) == 24 )
			{
				key = 3;
			}

			COLOR(BLACK, CORAL);
			LOCATE(49, 24);
			printf("[] NO []");

			if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && loop == 0 )
			{
				INPCLEAR();
				LOCATE(12, 21);
				printf("���̃L�����N�^�[�̃f�[�^�͕ۑ�����܂��񂪂�낵���ł����H");
				loop = 1;
				WAIT(250);
			}
			if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && loop == 1 )
			{
				INPCLEAR();
				COLOR(WHITE, BLACK);
				LOCATE(12, 21);
				printf("            ���̃L�����N�^�[���Z�[�u���܂����H            ");
				loop = 0;
				WAIT(250);
			}
		}
		else
		{
			COLOR(WHITE, BLACK);
			LOCATE(49, 24);
			printf("[] NO []");
		}
		WAIT(50);
	}
	
	rewind(stdin);
	getchar();

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : save
// �Ԃ�l : 0 
// ����   : ������L�����N�^�[�̃Z�[�u�f�[�^�쐬
////////////////////////////////////////////////////////////////////////////
int save (PLAYER *player)
{
	//�֐��錾
	int x, y;		//AA�̎�
	int key = 0;	//�L�[�{�[�h���쐧��
	int CNTdata;	//�Z�[�u�f�[�^�J�E���g
	int NUMdata[6];	//�Z�[�u�f�[�^�̗L��
	int END = 0;	//while������̒E�o
	char display[9][25][108] =	//�Z�[�u���AA
	{
		{
		   //12345678911234567892123456789312345678941234567895123456789612345678971234567898
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
			":|�P�P�P�P�P�P�P�P�P|:::::::::|�P�P�P�P�P�P�P�P�P|:::::::::|�P�P�P�P�P�P�P�P�P|:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|==================|:::::::::|==================|:::::::::|==================|:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|__________________|:::::::::|__________________|:::::::::|__________________|:",
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
			":|�P�P�P�P�P�P�P�P�P|:::::::::|�P�P�P�P�P�P�P�P�P|:::::::::|�P�P�P�P�P�P�P�P�P|:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|==================|:::::::::|==================|:::::::::|==================|:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|                  |:::::::::|                  |:::::::::|                  |:",
			":|__________________|:::::::::|__________________|:::::::::|__________________|:",
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
			":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[] ESC�Ŗ߂� []::",
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
		},

		{
			"�̗�     : ",
			"�U��     : ",
			"�h��     : ",
			"���΂₳ : ",
		},

		{
			"[ EMPTY ]" 
		},

		{
			"|�P�P�P�P�P�P�P�P�P|",
			"|                  |",
			"|                  |",
			"|==================|",
			"|                  |",
			"|                  |",
			"|                  |",
			"|                  |",
			"|                  |",
			"|__________________|",
		},

		{
			"[]                                                  []",
			"[]      �f�[�^���㏑�����܂�����낵���ł����H      []",
			"[]                                                  []",
			"[]                                                  []",
			"[]                                                  []",
		},

		{
				"[] YES []",
		},

		{
				"[] NO []",
		},

		{
			"[]                   []",
			"[] �Z�[�u����܂����B[]", 
			"[]                   []",
		},

		{
			"[]   �߂�   []",
		},
	};
	char *PsaveData[6] =		//�Z�[�u�f�[�^
	{
		"data1.bin",
		"data2.bin",
		"data3.bin",
		"data4.bin",
		"data5.bin",
		"data6.bin"
	};

	FILE *fp;

	//-------------------------------
	// �Z�[�u��ʍ쐬
	//-------------------------------
	CLS(WHITE, BLACK);
	for(y = 0; y < 25; y++)
	{
		LOCATE(1, y + 1);
		for(x = 0; x < 79; x++)
		{
			printf(":");
		}
	}

	//�Z�[�u�f�[�^�̗L���m�F
	for(CNTdata = 0; CNTdata < 6; CNTdata++)
	{
		player++;
		fp = fopen( PsaveData[CNTdata], "rb" );

		if(fp != 0)
		{
			NUMdata[CNTdata] = 0;
			fread( player, sizeof(PLAYER), 1, fp );
			fclose(fp);
		}
		else
		{
			NUMdata[CNTdata] = 1;
		}
	}
	player -= 6;

	INPCLEAR();
	while(1)
	{
		//�L�[�{�[�h���쏈��
		if( INP(PK_LEFT) != 0 )
		{
			key = ( key + 6 ) % 7;
			WAIT(80);
		}
		if( INP(PK_RIGHT) != 0 )
		{
			key = ( key + 1 ) % 7;
			WAIT(80);
		}
		if( INP(PK_UP) != 0 )
		{
			key = ( key + 6 ) % 9;
			WAIT(80);
		}
		if( INP(PK_DOWN) != 0 )
		{
			key = ( key + 3 ) % 9;
			WAIT(80);
		}

		for(CNTdata = 0; CNTdata < 6; CNTdata++)
		{
			//�J�[�\�����d�Ȃ��Ă�ꍇ
			if( key == CNTdata && ( INP(PM_CURX) >= 21 + 29 * (CNTdata % 3) || INP(PM_CURX) <= 2 + 29 * (CNTdata % 3)
				|| INP(PM_CURY) >= 11 + 11 * (CNTdata / 3) || INP(PM_CURY) <= 2 + 11 * (CNTdata / 3) )
				|| INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
			{
				//�}�E�X����̏ꍇ�L�[�{�[�h�ꎞ������
				if( INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
				{
					key = 13;
				}

				//�I�����ꂽ�ꍇ
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] == 0 )
				{
					for( y = 0; y < 5; y++ )
					{
						LOCATE(12, 10 + y);
						for( x = 0; x < 54; x ++ )
						{
							printf("%c",display[4][y][x]);
						}
					}

					INPCLEAR();
					END = 0;
					while( END == 0 )
					{
						//�J�[�\����YES�̏�ɂ���ꍇ
						if( INP(PM_CURX) >= 25 && INP(PM_CURX) <= 33
							&& INP(PM_CURY) ==13)
						{
							COLOR(BLACK, CORAL);
							LOCATE(25, 13);
							for( x = 0; x < 9; x++ )
							{
								printf("%c",display[5][0][x]);
							}

							//�N���b�N���ꂽ�ꍇ
							if( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 )
							{
								fp = fopen( PsaveData[CNTdata ], "wb" ); 
								if( fp == 0 )
								{
									printf("%c���J���܂���ł����B",PsaveData[CNTdata]);
									rewind(stdin);
									getchar();
									exit(0);
								}

								fwrite( player, sizeof(PLAYER), 1, fp );

								COLOR(BLACK, CORAL);
								for( y = 0; y < 3; y++)
								{
									LOCATE(28, 11 + y);
									for( x = 0; x < 23; x++)
									{
										printf("%c",display[7][y][x]);
									}
								}
								fclose(fp);
								WAIT(1000);
								return 0;
							}
						}
						else
						{
							COLOR(WHITE, BLACK);
							LOCATE(25, 13);
							for( x = 0; x < 9; x++ )
							{
								printf("%c",display[5][0][x]);
							}
						}

						//�J�[�\����NO�̏�ɂ���ꍇ
						if( INP(PM_CURX) >= 45 && INP(PM_CURX) <= 53
							&& INP(PM_CURY) ==13)
						{
							//�J�[�\������ɂ���ꍇ
							COLOR(BLACK, CORAL);
							LOCATE(45, 13);
							for( x = 0; x < 8; x++ )
							{
								printf("%c",display[6][0][x]);
							}

							//�N���b�N���ꂽ�ꍇ
							if( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 )
							{
								END = 1;
								CLS(WHITE, BLACK);
								for(y = 0; y < 25; y++)
								{
									LOCATE(1, y + 1);
									for(x = 0; x < 79; x++)
									{
										printf(":");
									}
								}

								//�͂�����
								for(y = 0; y < 10; y++)
								{
									if(CNTdata < 3)
									{
										LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
									}
									else
									{
										LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
									}
									for(x = 0; x < 20; x++)
									{
										printf("%c",display[3][y][x]);
									}
								}
								
								//�͂����̕������� �Z�[�u�f�[�^������ꍇ
								if(NUMdata[CNTdata] == 0)
								{
									if( CNTdata < 3 )
									{
										LOCATE( 4 + 29 * (CNTdata % 3), 3);
									}
									else
									{
										LOCATE( 4 + 29 * (CNTdata % 3), 14);
									}
									printf("NAME : %s",(player + CNTdata)->Name);

									for(y = 0; y < 4; y++)
									{
										if( CNTdata < 3 )
										{
											LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
										}
										else
										{
											LOCATE(4 + 29 * (CNTdata % 3), 18 + y);
										}
										for(x = 0; x < 11; x++)
										{
											printf("%c",display[1][y][x]);
										}

										switch(y)
										{
										case 0:
											{
												printf("%d",( player + CNTdata + 1)->Life);
												break;
											}

										case 1:
											{
												printf("%d",( player + CNTdata + 1)->Attack);
												break;
											}

										case 2:
											{
												printf("%d",( player + CNTdata + 1)->Def);
												break;
											}

										case 3:
											{
												printf("%d",( player + CNTdata + 1)->Speed);
												break;
											}
										}
									}
								}

								//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
								else
								{
									if( CNTdata < 3 )
									{
										LOCATE( 7 + 29 * (CNTdata % 3), 3);
									}
									else
									{
										LOCATE( 7 + 29 * (CNTdata % 3), 14);
									}
									for(x = 0; x < 9; x++)
									{
										printf("%c",display[2][0][x]);
									}
								}
							}//end of if
						}
						else
						{
							COLOR(WHITE, BLACK);
							LOCATE(45, 13);
							for( x = 0; x < 8; x++ )
							{
								printf("%c",display[6][0][x]);
							}
						}
						WAIT(50);
					}
				}

				//�Z�[�u�f�[�^���Ȃ������ꍇ
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] != 0 )
				{
					fp = fopen( PsaveData[CNTdata], "wb" ); 
					if( fp == 0 )
					{
						printf("%c���J���܂���ł����B",PsaveData[CNTdata]);
						rewind(stdin);
						getchar();
						exit(0);
					}

					fwrite( player, sizeof(PLAYER), 1, fp );

					COLOR(BLACK, CORAL);
					for( y = 0; y < 3; y++)
					{
						LOCATE(28, 11 + y);
						for( x = 0; x < 23; x++)
						{
							printf("%c",display[7][y][x]);
						}
					}
					fclose(fp);
					WAIT(1000);
					return 0;
				}

				COLOR(BLACK, CORAL);

				//�͂�����
				for(y = 0; y < 10; y++)
				{
					if(CNTdata < 3)
					{
						LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
					}
					else
					{
						LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
					}
					for(x = 0; x < 20; x++)
					{
						printf("%c",display[3][y][x]);
					}
				}
				
				//�͂����̕������� �Z�[�u�f�[�^������ꍇ
				if(NUMdata[CNTdata] == 0)
				{
					if( CNTdata < 3 )
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 14);
					}
					printf("NAME : %s",(player + CNTdata + 1)->Name);

					for(y = 0; y < 4; y++)
					{
						if( CNTdata < 3 )
						{
							LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
						}
						else
						{
							LOCATE(4 + 29 * (CNTdata % 3), 18 + y);
						}
						for(x = 0; x < 11; x++)
						{
							printf("%c",display[1][y][x]);
						}

						switch(y)
						{
						case 0:
							{
								printf("%d",( player + CNTdata + 1)->Life);
								break;
							}

						case 1:
							{
								printf("%d",( player + CNTdata + 1)->Attack);
								break;
							}

						case 2:
							{
								printf("%d",( player + CNTdata + 1)->Def);
								break;
							}

						case 3:
							{
								printf("%d",( player + CNTdata + 1)->Speed);
								break;
							}
						}
					}
				}

				//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
				else
				{
					if( CNTdata < 3 )
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 14);
					}
					for(x = 0; x < 9; x++)
					{
						printf("%c",display[2][0][x]);
					}
				}
			}//end of if

			//�J�[�\�����d�Ȃ��ĂȂ��ꍇ
			else
			{
				COLOR(WHITE, BLACK);

				//�͂�����
				for(y = 0; y < 10; y++)
				{
					if(CNTdata < 3)
					{
						LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
					}
					else
					{
						LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
					}
					for(x = 0; x < 20; x++)
					{
						printf("%c",display[3][y][x]);
					}
				}
				
				//�͂����̕������� �Z�[�u�f�[�^������ꍇ
				if(NUMdata[CNTdata] == 0)
				{
					if( CNTdata < 3 )
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 14);
					}
					printf("NAME : %s",(player + CNTdata + 1)->Name);

					for(y = 0; y < 4; y++)
					{
						if( CNTdata < 3 )
						{
							LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
						}
						else
						{
							LOCATE(4 + 29 * (CNTdata % 3), 18 + y);
						}
						for(x = 0; x < 11; x++)
						{
							printf("%c",display[1][y][x]);
						}

						switch(y)
						{
						case 0:
							{
								printf("%d",( player + CNTdata + 1)->Life);
								break;
							}

						case 1:
							{
								printf("%d",( player + CNTdata + 1)->Attack);
								break;
							}

						case 2:
							{
								printf("%d",( player + CNTdata + 1)->Def);
								break;
							}

						case 3:
							{
								printf("%d",( player + CNTdata + 1)->Speed);
								break;
							}
						}
					}
				}

				//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
				else
				{
					if( CNTdata < 3 )
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 14);
					}
					for(x = 0; x < 9; x++)
					{
						printf("%c",display[2][0][x]);
					}
				}
			}//end of else
		}//end of for

		//ESC�ɃJ�[�\�����d�Ȃ�����
		if( ( key >= 6 && key <= 9 ) && ( INP(PM_CURX) <= 65 || INP(PM_CURX) >= 83
			|| INP(PM_CURY) != 24)
			|| INP(PM_CURX) <= 83 && INP(PM_CURX) >= 65
			&& INP(PM_CURY) == 24 )
		{
			if( INP(PM_CURX) <= 83 && INP(PM_CURX) >= 65
			&& INP(PM_CURY) == 24 )
			{
				key = 12;
			}

			COLOR(BLACK, CORAL);
			LOCATE(65, 24);
			for( x = 0; x < 14; x++ )
			{
				printf("%c",display[8][0][x]);
			}

			//�N���b�N���ꂽ�ꍇ
			if( INP(PM_LEFT) != 0 || INP(PK_ESC) != 0 )
			{
				break;
			}
		}
		//ESC�ɃJ�[�\�����d�Ȃ��ĂȂ�������
		else
		{
			COLOR(WHITE, BLACK);
			LOCATE(65, 24);
			for( x = 0; x < 14; x++ )
			{
				printf("%c",display[8][0][x]);
			}
		}
		WAIT(50);
	}//end of while

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : load
// �Ԃ�l : 0, 1
// ����   : �Z�[�u�f�[�^�̃��[�h
////////////////////////////////////////////////////////////////////////////
int load (PLAYER *player)
{
	//�֐��錾
	int x, y;		//AA�̎�
	int key = 0;	//�L�[�{�[�h���쐧��
	int CNTdata;	//�Z�[�u�f�[�^�J�E���g
	int NUMdata[6];	//�Z�[�u�f�[�^�̗L��
	int END = 0;	//while������̒E�o
	char display[8][25][108] =	//�Z�[�u���AA
	{
		{
			"�̗�     : ",
			"�U��     : ",
			"�h��     : ",
			"���΂₳ : ",
		},

		{
			"[ EMPTY ]" 
		},

		{
			"|�P�P�P�P�P�P�P�P�P|",
			"|                  |",
			"|                  |",
			"|==================|",
			"|                  |",
			"|                  |",
			"|                  |",
			"|                  |",
			"|                  |",
			"|__________________|",
		},

		{
			"[]                                                  []",
			"[]      �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@      []",
			"[]             �Z�[�u�f�[�^������܂���             []",
			"[]                                                  []",
			"[]                                                  []",
		},

		{
				"[] YES []",
		},

		{
				"[] NO []",
		},

		{
			"[]                   []",
			"[] ���[�h����܂����B[]", 
			"[]                   []",
		},

		{
			"[]   �߂�   []",
		},
	};
	char *PsaveData[6] =	//�Z�[�u�f�[�^
	{
		"data1.bin",
		"data2.bin",
		"data3.bin",
		"data4.bin",
		"data5.bin",
		"data6.bin"
	};

	FILE *fp;

	//-------------------------------
	// ���[�h��ʍ쐬
	//-------------------------------
	CLS(WHITE, BLACK);
	for(y = 0; y < 25; y++)
	{
		LOCATE(1, y + 1);
		for(x = 0; x < 79; x++)
		{
			printf(":");
		}
	}

	//�Z�[�u�f�[�^�̗L���m�F
	for(CNTdata = 0; CNTdata < 6; CNTdata++)
	{
		player++;
		fp = fopen( PsaveData[CNTdata], "rb" );

		if(fp != 0)
		{
			NUMdata[CNTdata] = 0;
			fread( player, sizeof(PLAYER), 1, fp );
			fclose(fp);
		}
		else
		{
			NUMdata[CNTdata] = 1;
		}
	}
	player -= 6;

	INPCLEAR();
	while(1)
	{
		//�L�[�{�[�h���쏈��
		if( INP(PK_LEFT) != 0 )
		{
			key = ( key + 6 ) % 7;
			WAIT(80);
		}
		if( INP(PK_RIGHT) != 0 )
		{
			key = ( key + 1 ) % 7;
			WAIT(80);
		}
		if( INP(PK_UP) != 0 )
		{
			key = ( key + 6 ) % 9;
			WAIT(80);
		}
		if( INP(PK_DOWN) != 0 )
		{
			key = ( key + 3 ) % 9;
			WAIT(80);
		}

		for(CNTdata = 0; CNTdata < 6; CNTdata++)
		{
			//�J�[�\�����d�Ȃ��Ă�ꍇ
			if( key == CNTdata && ( INP(PM_CURX) >= 21 + 29 * (CNTdata % 3) || INP(PM_CURX) <= 2 + 29 * (CNTdata % 3)
				|| INP(PM_CURY) >= 11 + 11 * (CNTdata / 3) || INP(PM_CURY) <= 2 + 11 * (CNTdata / 3) )
				|| INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
			{
				//�}�E�X����̏ꍇ�L�[�{�[�h�ꎞ������
				if( INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
				{
					key = 13;
				}

				//�Z�[�u�f�[�^���������ꍇ
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] == 0 )
				{
					fp = fopen( PsaveData[CNTdata], "rb" );
					if(fp == 0)
					{
						printf("%c�����[�h����܂���ł����B",PsaveData[CNTdata]);
						rewind(stdin);
						getchar();
					}

					fread( player, sizeof(PLAYER), 1, fp );
					for( y = 0; y < 3; y++ )
					{
						LOCATE(28, 11 + y);
						for( x = 0; x < 23; x++ )
						{
							printf("%c",display[6][y][x]);
						}
					}
					WAIT(1000);
					fclose(fp);
					return 1;
				}

				//�Z�[�u�f�[�^���Ȃ������ꍇ
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] != 0 )
				{
					COLOR(BLACK, CORAL);
					for( y = 0; y < 5; y++)
					{
						LOCATE( 12, 10 + y );
						for( x = 0; x < 54; x++ )
						{
							printf("%c",display[3][y][x]);
						}
					}
					WAIT(1000);

					COLOR(WHITE, BLACK);

					for( y = 0; y < 25; y++ )
					{
						LOCATE(1, 1 + y);
						for( x = 0; x < 79; x++)
						{
							printf(":");
						}
					}

					//�͂�����
					for(y = 0; y < 10; y++)
					{
						if(CNTdata < 3)
						{
							LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
						}
						else
						{
							LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
						}
						for(x = 0; x < 20; x++)
						{
							printf("%c",display[2][y][x]);
						}
					}
					
					//�͂����̕������� �Z�[�u�f�[�^������ꍇ
					if(NUMdata[CNTdata] == 0)
					{
						if( CNTdata < 3 )
						{
							LOCATE( 4 + 29 * (CNTdata % 3), 3);
						}
						else
						{
							LOCATE( 4 + 29 * (CNTdata % 3), 14);
						}
						printf("NAME : %s",(player + CNTdata + 1)->Name);

						for(y = 0; y < 4; y++)
						{
							if( CNTdata < 3 )
							{
								LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
							}
							else
							{
								LOCATE(4 + 29 * (CNTdata % 3), 17 + y);
							}
							for(x = 0; x < 11; x++)
							{
								printf("%c",display[0][y][x]);
							}

							switch(y)
							{
							case 0:
								{
									printf("%d",( player + CNTdata + 1)->Life);
									break;
								}

							case 1:
								{
									printf("%d",( player + CNTdata + 1)->Attack);
									break;
								}

							case 2:
								{
									printf("%d",( player + CNTdata + 1)->Def);
									break;
								}

							case 3:
								{
									printf("%d",( player + CNTdata + 1)->Speed);
									break;
								}
							}
						}
					}

					//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
					else
					{
						if( CNTdata < 3 )
						{
							LOCATE( 7 + 29 * (CNTdata % 3), 3);
						}
						else
						{
							LOCATE( 7 + 29 * (CNTdata % 3), 16);
						}
						for(x = 0; x < 9; x++)
						{
							printf("%c",display[1][0][x]);
						}
					}
				}

				COLOR(BLACK, CORAL);

				//�͂�����
				for(y = 0; y < 10; y++)
				{
					if(CNTdata < 3)
					{
						LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
					}
					else
					{
						LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
					}
					for(x = 0; x < 20; x++)
					{
						printf("%c",display[2][y][x]);
					}
				}
				
				//�͂����̕������� �Z�[�u�f�[�^������ꍇ
				if(NUMdata[CNTdata] == 0)
				{
					if( CNTdata < 3 )
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 14);
					}
					printf("NAME : %s",(player + CNTdata + 1)->Name);

					for(y = 0; y < 4; y++)
					{
						if( CNTdata < 3 )
						{
							LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
						}
						else
						{
							LOCATE(4 + 29 * (CNTdata % 3), 18 + y);
						}
						for(x = 0; x < 11; x++)
						{
							printf("%c",display[0][y][x]);
						}

						switch(y)
						{
						case 0:
							{
								printf("%d",( player + CNTdata + 1)->Life);
								break;
							}

						case 1:
							{
								printf("%d",( player + CNTdata + 1)->Attack);
								break;
							}

						case 2:
							{
								printf("%d",( player + CNTdata + 1)->Def);
								break;
							}

						case 3:
							{
								printf("%d",( player + CNTdata + 1)->Speed);
								break;
							}
						}
					}
				}

				//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
				else
				{
					if( CNTdata < 3 )
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 14);
					}
					for(x = 0; x < 9; x++)
					{
						printf("%c",display[1][0][x]);
					}
				}
			}//end of if

			//�J�[�\�����d�Ȃ��ĂȂ��ꍇ
			else
			{
				COLOR(WHITE, BLACK);

				//�͂�����
				for(y = 0; y < 10; y++)
				{
					if(CNTdata < 3)
					{
						LOCATE(2 + 29 * (CNTdata % 3), 2 + y);
					}
					else
					{
						LOCATE(2 + 29 * (CNTdata % 3), 13 + y);
					}
					for(x = 0; x < 20; x++)
					{
						printf("%c",display[2][y][x]);
					}
				}
				
				//�͂����̕������� �Z�[�u�f�[�^������ꍇ
				if(NUMdata[CNTdata] == 0)
				{
					if( CNTdata < 3 )
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 4 + 29 * (CNTdata % 3), 14);
					}
					printf("NAME : %s",(player + CNTdata + 1)->Name);

					for(y = 0; y < 4; y++)
					{
						if( CNTdata < 3 )
						{
							LOCATE(4 + 29 * (CNTdata % 3), 7 + y);
						}
						else
						{
							LOCATE(4 + 29 * (CNTdata % 3), 18 + y);
						}
						for(x = 0; x < 11; x++)
						{
							printf("%c",display[0][y][x]);
						}

						switch(y)
						{
						case 0:
							{
								printf("%d",( player + CNTdata + 1)->Life);
								break;
							}

						case 1:
							{
								printf("%d",( player + CNTdata + 1)->Attack);
								break;
							}

						case 2:
							{
								printf("%d",( player + CNTdata + 1)->Def);
								break;
							}

						case 3:
							{
								printf("%d",( player + CNTdata + 1)->Speed);
								break;
							}
						}
					}
				}

				//�͂����̕������� �Z�[�u�f�[�^���Ȃ��ꍇ
				else
				{
					if( CNTdata < 3 )
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 3);
					}
					else
					{
						LOCATE( 7 + 29 * (CNTdata % 3), 14);
					}
					for(x = 0; x < 9; x++)
					{
						printf("%c",display[1][0][x]);
					}
				}
			}//end of else
		}//end of for

		//ESC�ɃJ�[�\�����d�Ȃ�����
		if( ( key >= 6 && key <= 9 ) && ( INP(PM_CURX) <= 65 || INP(PM_CURX) >= 83
			|| INP(PM_CURY) != 24)
			|| INP(PM_CURX) <= 83 && INP(PM_CURX) >= 65
			&& INP(PM_CURY) == 24 )
		{
			if( INP(PM_CURX) <= 83 && INP(PM_CURX) >= 65
			&& INP(PM_CURY) == 24 )
			{
				key = 12;
			}

			COLOR(BLACK, CORAL);
			LOCATE(65, 24);
			for( x = 0; x < 15; x++ )
			{
				printf("%c",display[7][0][x]);
			}

			//�N���b�N���ꂽ�ꍇ
			if( INP(PM_LEFT) != 0 || INP(PK_ESC) != 0 )
			{
				return 0;
			}
		}
		//ESC�ɃJ�[�\�����d�Ȃ��ĂȂ�������
		else
		{
			COLOR(WHITE, BLACK);
			LOCATE(65, 24);
			for( x = 0; x < 15; x++ )
			{
				printf("%c",display[7][0][x]);
			}
		}
		WAIT(50);
	}//end of while
	exit(0);
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : battle
// �Ԃ�l :
// ����   : �N�������܂ŉ���̂���߂Ȃ��b�I�I
////////////////////////////////////////////////////////////////////////////
int battleAnime (PLAYER *player)
{
	//�֐��錾
	int x, y;		//AA��X���W�AY���W
	int CNTchar;	//for���̐���
	bool endBattle;	//�킢���I��������ǂ���
	int turn;		//�U���t�F�[�Y����
	int defoHP[2];	//�f�t�H���g��HP
	int damage = 0;	//�_���[�W
	char display[10][25][100] =
	{
		{
			"][                                                                           ][",
			"[]                                                                           []",
			"[]                                                                           ][",
			"][                                                                           []",
			"[]                                                                           ][",
			"][                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
			"[]                                                                           []",
			"][                                                                           ][",
		},
		
		{
			"    ____________    ",
			"  �^            �_  ",
			" ::               ` ",
			"::::               |",
			":::::              |",
			":::::.             |",
		},

		{
			"    ___________     ",
			"   /           `    ",
			"  |   ||   ||   |   ",
			"  |     �[      |   ",
			"|�_ . ...      /    ",
			" �_�_        :      ",
			"   �_�_�^    |      ",
			"    :__       :     ",
			"    |_______ __:    ",
			"     ||     ||      ",
			" ____ ||_____||_____",
		},

		{
			"�P�P�P�P�P�P|",
			"            |",
			"____________|",
		},

		{
			"|�P�P�P�P�P�P",
			"|            ",
			"|____________",
		},

		{
			",,     ,,  ",
			";;:   ;;/  ",
			" ;;  ;;/   ",
			"  ; ;;/    ",
			"   ;;/     ",
			"   '       ",
		},

		{
			" .;;, ",
			"::  ::",
			" :,,  ",
			"    ;;",
			";;  ;;",
			"';;'  ",
		},

		{
			"  ______________",
			"�^  BATTLE LOG  ",
		},

		{
			"[]        ###         ####      ####       ####   ##########     []",
			"[]      ##   ##      ##  ##     ## ##     ## ##   ##             []",
			"[]     ##     ##    ##    ##    ## ##     ## ##   ##             []",
			"[]     ##     ##   ##      ##   ##  ##   ##  ##   ##########     []",
			"[]     ##          ##########   ##  ##   ##  ##   ##             []",
			"[]     ##   ####   ##      ##   ##   ## ##   ##   ##             []",
			"[]      ##   ###   ##      ##   ##   ## ##   ##   ##             []",
			"[]        ### ##   ##      ##   ##    ###    ##   ##########     []",
			"[]                                                               []",
			"[]                                                               []",
			"[]          ###      ##       ##   #########    ########         []",
			"[]        ##   ##    ##       ##   ##           ##     ##        []",
			"[]       ##     ##    ##     ##    ##           ##       #       []",
			"[]       ##     ##    ##     ##    #########    ##     ##        []",
			"[]       ##     ##     ##   ##     ##           ########         []",
			"[]       ##     ##     ##   ##     ##           ##  ##           []",
			"[]        ##   ##       ## ##      ##           ##    ##         []",
			"[]          ###         #####      #########    ##      ##       []",
		},

		{
			"[]        ###         ####      ####       ####   ##########     []",
			"[]      ##   ##      ##  ##     ## ##     ## ##   ##             []",
			"[]     ##     ##    ##    ##    ## ##     ## ##   ##             []",
			"[]     ##     ##   ##      ##   ##  ##   ##  ##   ##########     []",
			"[]     ##          ##########   ##  ##   ##  ##   ##             []",
			"[]     ##   ####   ##      ##   ##   ## ##   ##   ##             []",
			"[]      ##   ###   ##      ##   ##   ## ##   ##   ##             []",
			"[]        ### ##   ##      ##   ##    ###    ##   ##########     []",
			"[]                                                               []",
			"[]                                                               []",
			"[]     ###      ##         ########      ####      ########      []",
			"[]   ##   ##    ##         ##           ##  ##     ##     ##     []",
			"[]  ##     ##   ##         ##          ##    ##    ##       #    []",
			"[]  ##          ##         ########   ##      ##   ##     ##     []",
			"[]  ##          ##         ##         ##########   ########      []",
			"[]  ##     ##   ##         ##         ##      ##   ##  ##        []",
			"[]   ##   ##    ##         ##         ##      ##   ##    ##      []",
			"[]     ###      ########   ########   ##      ##   ##      ##    []",
		},
	};
	char message[5][40] =
	{
		"�͉����ڂ����Ă���",
		"�͗]�T�����Ȋ�����Ă���",
		"�͂Ԃ�ȓ��Ō��߂Ă���",
		"�uI want to win the beatbox battle�v",

	};
	srand((unsigned) time (NULL));		//�����̎�܂�

	//-------------------------------
	// �퓬��ʍ쐬
	//-------------------------------
	CUROFF();
	CLS(WHITE, BLACK);
	for( y = 0; y < 25; y++ )
	{
		LOCATE(1, 1 + y);
		for( x = 0; x < 79; x++ )
		{
			printf("%c",display[0][y][x]);
		}
	}
	WAIT(500);

	//BATTLE LOG
	LOCATE(3, 21);
	for( x = 0; x < 29; x++ )
	{
		printf("�P");
	}

	for( y = 0; y < 2; y++ )
	{
		LOCATE(62, 19 + y);
		for( x = 0; x < 16; x++ )
		{
			printf("%c",display[7][y][x]);
		}
	}

	//�L�����N�^�[��
	for( y = 0; y < 3; y++ )
	{
		//�v���C���[
		LOCATE(3, 3 + y);
		for( x = 0; x < 13; x++ )
		{
			printf("%c",display[3][y][x]);
		}

		//�G
		LOCATE(65, 15 + y);
		for( x = 0; x < 13; x++ )
		{
			printf("%c",display[4][y][x]);
		}
	}
	LOCATE(4, 4);
	printf("%s",(player + 1)->Name);
	LOCATE(66, 16);
	printf("%s",player->Name);

	//HP
	defoHP[0] = player->Life;
	defoHP[1] = (player + 1)->Life;
	LOCATE(17, 4);
	printf("HP%4d / %4d",defoHP[1], (player + 1)->Life);
	LOCATE(50, 16);
	printf("HP%4d / %4d",defoHP[0], player->Life);

	//�G�o��
	COLOR(CORAL);
	for( CNTchar = 0; CNTchar < 20; CNTchar++ )
	{
		for( y = 0; y < 11; y++ )
		{
			LOCATE(77 - CNTchar, 1 + y);
			for( x = 0; x < CNTchar; x++ )
			{
				printf("%c",display[2][y][x]);

				/*if( CNTchar != 0 )
				{
					LOCATE(77 - x, 5 + y);

					printf(" ");
				}*/
			}
		}
		WAIT(50);
	}

	//�G�ړ�
	for( CNTchar = 0; CNTchar < 5; CNTchar++)
	{
		for( y = 0; y < 11; y++ )
		{
			LOCATE(57 - CNTchar, 1 + y);
			for( x = 0; x < 20; x++ )
			{
				printf("%c",display[2][y][x]);
			}
			printf(" ");
		}
		WAIT(50);
	}
	WAIT(500);

	//�v���C���[�o��
	COLOR(WHITE);
	for( CNTchar = 20; CNTchar > 0; CNTchar-- )
	{
		for( y = 0; y < 6; y++ )
		{
			LOCATE(3, 15 + y);
			for( x = 0; x < 20 - CNTchar; x++ )
			{
				printf("%c",display[1][y][CNTchar + x]);
			}
		}
		WAIT(50);
	}

	//�v���C���[�ړ�
	for( CNTchar = 0; CNTchar  < 5; CNTchar++ )
	{
		for( y = 0; y < 6; y++ )
		{
			LOCATE(3 + CNTchar, 15 + y );
			printf(" ");
			for( x = 0; x < 20; x++ )
			{
				printf("%c",display[1][y][x]);
			}
		}
		WAIT(50);
	}

	LOCATE(3, 22);
	printf("%s",(player + 1)->Name);
	switch( rand() % 4 )
	{
	case 0:
		{
			for( x = 0; x < 18; x++ )
			{
				printf("%c",message[0][x]);
			}
			break;
		}
		
	case 1:
		{
			for( x = 0; x < 24; x++ )
			{
				printf("%c",message[1][x]);
			}
			break;
		}

	case 2:
		{
			for( x = 0; x < 26; x++ )
			{
				printf("%c",message[2][x]);
			}
			break;
		}

	case 3:
		{
			for( x = 0; x < 36; x++ )
			{
				printf("%c",message[3][x]);
			}
			break;
		}

	default:
		{
			printf("miss");
			break;
		}
	}

	//�N���b�N��Enter�҂�
	INPCLEAR();
		while(1)
		{
			if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
			{
				WAIT(50);
				break;
			}
		}

	//-------------------------------
	//�o�g������
	//-------------------------------
	if(player->Speed < (player+1)->Speed)
	{
		turn = 1;
	}
	endBattle = false;
	if ( player->Speed > ( player + 1 )->Speed)
	{
		turn = 0;
	}
	else
	{
		turn = 1;
	}

	while(endBattle == false)
	{
		//BATTLE LOG������
		for( y = 0; y < 3; y++ )
		{
			LOCATE(3, 22 + y);
			for( x = 0; x < 75; x++ )
			{
				printf(" ");
			}
		}

		endBattle = battle( ( player + turn ) , ( player + (turn^1) ));
		turn ^= 1;

		//HP
		LOCATE(17, 4);
		printf("HP%4d / %4d",(player + 1)->Life, defoHP[1]);
		LOCATE(50, 16);
		printf("HP%4d / %4d",player->Life, defoHP[0]);
	}

	//-------------------------------
	// �o�g���G���h����
	//-------------------------------
	WAIT(1500);
	if(player->Life == 0)
	{
		COLOR(BLACK, CORAL);
		for( y = 0; y < 18; y++ )
		{
			LOCATE(6, 3 + y);
			for( x = 0; x < 67; x++ )
			{
				printf("%c",display[8][y][x]);
			}
		}
		return 0;
	}
	else
	{
		COLOR(BLACK, YELLOW);
		for( y = 0; y < 18; y++ )
		{
			LOCATE(6, 3 + y);
			for( x = 0; x < 67; x++ )
			{
				printf("%c",display[9][y][x]);
			}
		}
		return 1;
	}

	rewind(stdin);
	getchar();
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// �֐��� : battle
// �Ԃ�l : 
// ����   : �킪�n�܂�
////////////////////////////////////////////////////////////////////////////
bool battle ( PLAYER *ATCplayer, PLAYER *DEFplayer )
{
	//�֐��錾
	int avoidance;		//���
	int damage;			//�_���[�W
	int next;			//�N���b�N��Enter�҂�

	srand( ( unsigned )time( NULL ) );		//�����_��

	//-------------------------------
	// �퓬����
	//-------------------------------
	avoidance = rand();
	
	/*if( ( avoidance / 600 ) * DEFplayer->Speed < ( rand() * rand() / 2 ) % avoidance )
	{
		printf("%s�̍U���͉�����ꂽ",ATCplayer->Name);
		WAIT(700);
		return false;
	}*/
	//else
	//{
		damage = ATCplayer->Attack - ( ATCplayer->Attack * ( DEFplayer->Def / 600 ) );
		LOCATE(3, 22);
		printf("%s�̍U��",ATCplayer->Name);
		WAIT(600);
		LOCATE(3, 23);
		printf("%s��%d�̃_���[�W���󂯂�",DEFplayer->Name ,damage);

		DEFplayer->Life -= ATCplayer->Attack;
		if( DEFplayer->Life < 0 )
		{
			DEFplayer->Life = 0;
		}

		//�N���b�N��Enter�҂�
		INPCLEAR();
		while(1)
		{
			if(INP(PK_ENTER) != 0 || INP(PM_LEFT) != 0)
			{
				WAIT(50);
				break;
			}
		}
		

		if ( DEFplayer->Life == 0 )
		{
			return true;
		}
		return false;
	//}
}