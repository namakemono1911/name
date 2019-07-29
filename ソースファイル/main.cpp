/////////////////////////////////////////////////////////////////////////////
//	メイン処理 (main.cpp)
//	Author : TAKUMI UENO
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
//	インクルードファイル
//--------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CScreen.h"

//--------------------------------------------------------------------------
//	マクロ定義
//--------------------------------------------------------------------------
#define MAX_NAME (10)			//キャラクター名の最大文字数
#define MAX_PLAYER (2)			//プレイヤーの最大数

//--------------------------------------------------------------------------
//	構造体定義
//--------------------------------------------------------------------------
typedef struct
{
	char Name[MAX_NAME];		//キャラクターの名前
	int Life;					//キャラクターのHP
	int Attack;					//キャラクターの攻撃力
	int Def;					//キャラクターの防御力
	int Speed;					//キャラクターのすばやさ
}PLAYER;

enum SELECT
{
	CREATE,
	BATTLE,
	MAKE,
	END
};

//--------------------------------------------------------------------------
//　プロトタイプ宣言
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
// 関数名	: メイン関数
// 返り値	: 0
// 説明		: メイン処理
////////////////////////////////////////////////////////////////////////////
int  main( void )
{
	//関数宣言
	PLAYER player[7];			//セーブデータのキャラクター
	enum SELECT select;			//処理分岐
	int CNTname = 0;			//名前の文字数
	bool bEndBatttle = false;	//ブール型
	int nTurn = 0;				//ターン制の攻撃
	int check;					//セーブするか否か
	int result;					//戦闘結果
	int titleBGM;				//titleのBGM
	int makeBGM;				//make中のBGM
	int battleBGM;				//battle中のBGM
	int winBGM;					//勝利時のBGM
	int loseBGM;				//敗北時のBGM
	int root;					//敵を作るかプレイヤーを作るか

	//-------------------------------
	// BGMハンドル指定
	//-------------------------------
	/*titleBGM = OPENMP3("ここにタイトル画面で流したいMP3データを拡張子まで書いて下さい");
	if(titleBGM == 0)
	{
		CLS();
		printf("【error】タイトルBGMが開けませんでした。\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	makeBGM = OPENMP3("ここにメイキング画面で流したいMP3データを拡張子まで書いて下さい");
	if ( makeBGM ==0)
	{
		printf("【error】メイキングBGMが開けませんでした。\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	battleBGM = OPENMP3("ここにバトル画面で流したいMP3データを拡張子まで書いて下さい");
	if ( battleBGM ==0)
	{
		printf("【error】バトルBGMが開けませんでした。\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	winBGM = OPENMP3("ここに勝利時に流したいMP3データを拡張子まで書いて下さい");
	if ( battleBGM ==0)
	{
		printf("【error】勝利BGMが開けませんでした。\n");
		rewind(stdin);
		getchar();
		exit(0);
	}

	loseBGM = OPENMP3("ここに敗北時に流したいMP3データを拡張子まで書いて下さい");
	if ( battleBGM ==0)
	{
		printf("【error】敗北BGMが開けませんでした。\n");
		rewind(stdin);
		getchar();
		exit(0);
	}*/

	//-------------------------------
	//タイトル画面
	//-------------------------------
	CUROFF();
	while(1)
	{
		PLAYMP3(titleBGM, 1);
		select = title();
		//select = BATTLE;

		//-------------------------------
		// 動作選択
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

					//クリックかEnter待ち
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

					//クリックかEnter待ち
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
				printf("予想外のエラー");
				rewind(stdin);
				getchar();
				exit(0);
			}
		}
	}

	/*input(&player[0]);
	input(&player[1]);
	
	//-------------------------------
	//バトル処理
	//-------------------------------
	
	if(player->nSpeed < (player+1)->nSpeed)
	{
		nTurn = 1;
	}
	while(bEndBatttle == false)      //倒れるまで
	{
		Output(&player[0]);
		bEndBatttle = Battle(&player[nTurn] , &player[nTurn^1]);
		nTurn ^= 1;
	}*/


	// キーボタン押し待ちの処理 
	rewind( stdin );
	getchar();

	// 終了
	exit(0);
}

////////////////////////////////////////////////////////////////////////////
// 関数名	: title
// 返り値	: CREATE, BATTLE, END
// 説明		: タイトル画面作成
////////////////////////////////////////////////////////////////////////////
enum SELECT title ()
{
	//関数宣言
	int x,y;				//タイトルのx軸、y軸
	int count;				//タイトルのスペース
	int loop = 0;			//選択項目のループ
	char title[][25][89] =	//タイトル
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
			"[]￣￣￣￣￣￣￣￣￣|",
			"][      CREATE      |",
			"[]__________________|"
		},

		{
			"[]￣￣￣￣￣￣￣￣￣|",
			"][      BATTLE      |",
			"[]__________________|"
		},

		{
			"[]￣￣￣￣￣￣￣￣￣|",
			"][      END         |",
			"[]__________________|"
		},
	};

	//-------------------------------
	// タイトル画面出現
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
	// 選択項目出現
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
	// タイトルロゴ出現
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
	// 選択されたときの表示切替
	//-------------------------------
	INPCLEAR();
	while(1)
	{
		//キーボード操作
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
// 関数名	: create
// 返り値	: 0
// 説明		: キャラクターのステータス作成
////////////////////////////////////////////////////////////////////////////
int create(PLAYER *player, int root)
{
	//関数宣言
	int x,y;			//AAの軸
	int check;			//文字数チェック
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
		"][::::::::::::::::::::::[]         (10文字以内)         []::::::::::::::::::::][",
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
	// 名前入力
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
		printf("名前を入力してください。");
	}
	else
	{
		LOCATE(27, 10);
		printf("相手の名前を入力してください。");
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
			printf("名前が長すぎます。");
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
// 関数名 : calculation
// 返り値 : 0
// 説明   : ステータス計算
////////////////////////////////////////////////////////////////////////////
int calculation (PLAYER *player, int CNTname)
{
	//関数宣言
	int data = 0;			//キャラクター名を数値化したデータ
	int loop;				//for文制御

	//-------------------------------
	// 体力計算
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
	// 攻撃力計算
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
	// 防御力
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
	// すばやさ
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
// 関数名 : makeSta
// 返り値 : 0, 1
// 説明   : キャラクターのステータス生成
////////////////////////////////////////////////////////////////////////////
int makeSta (PLAYER *player)
{
	//関数宣言
	int loop;				//for文制御
	int key = 0;			//キーボード操作
	int x, y;				//AAの縦横
	char display[25][102] =
	{
		
			"][][][][][][][][][][][][][][][][][][][][][][][][][][::::][][                ][][",
			"[]             ][:::::::::::::::::::::[][][][][][][]::::[][][STATS]         [][]",
			"][  NAME:      []::::::::::::::::::::::[][][][][][][::::][][体力     :      ][][",
			"[]             ][:::::::::::::::::::::::[][][][][][]::::[][]::::::::::::::::[][]",
			"][             []::::::::::::::::::::::::[][][][][][::::][][攻撃     :      ][][",
			"[]             ] ／              ＼:::::::[][][][][]::::[][]::::::::::::::::[][]",
			"][][][]][][][][]|                 `::::::::[][][][][::::][][防御     :      ][][",
			"[][][]::::::::::|     []    []     |::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::|         -        |::::::::][][][][::::][][すばやさ :      ][][",
			"[][][]::::::::::|                  |::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][::::::::::::                ::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]:::::::::::::::.         .::::::::::::[][][][]::::[][]::::::::::::::::[][]",
			"][][][:::::::::::::／         ＼＼::::::::::][][][][::::][][::::::::::::::::][][",
			"[][][]:::::::::::⊂.:|        |:. ⊃::::::::[][][][]::::[][]::::::::::::::::[][]",
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
	// ステータス画面
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
	printf("このキャラクターをセーブしますか？");

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
				printf("このキャラクターのデータは保存されませんがよろしいですか？");
				loop = 1;
				WAIT(250);
			}
			if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && loop == 1 )
			{
				INPCLEAR();
				COLOR(WHITE, BLACK);
				LOCATE(12, 21);
				printf("            このキャラクターをセーブしますか？            ");
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
// 関数名 : save
// 返り値 : 0 
// 説明   : 作ったキャラクターのセーブデータ作成
////////////////////////////////////////////////////////////////////////////
int save (PLAYER *player)
{
	//関数宣言
	int x, y;		//AAの軸
	int key = 0;	//キーボード操作制御
	int CNTdata;	//セーブデータカウント
	int NUMdata[6];	//セーブデータの有無
	int END = 0;	//while文からの脱出
	char display[9][25][108] =	//セーブ画面AA
	{
		{
		   //12345678911234567892123456789312345678941234567895123456789612345678971234567898
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
			":|￣￣￣￣￣￣￣￣￣|:::::::::|￣￣￣￣￣￣￣￣￣|:::::::::|￣￣￣￣￣￣￣￣￣|:",
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
			":|￣￣￣￣￣￣￣￣￣|:::::::::|￣￣￣￣￣￣￣￣￣|:::::::::|￣￣￣￣￣￣￣￣￣|:",
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
			":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::[] ESCで戻る []::",
			"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::",
		},

		{
			"体力     : ",
			"攻撃     : ",
			"防御     : ",
			"すばやさ : ",
		},

		{
			"[ EMPTY ]" 
		},

		{
			"|￣￣￣￣￣￣￣￣￣|",
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
			"[]      データを上書きしますがよろしいですか？      []",
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
			"[] セーブされました。[]", 
			"[]                   []",
		},

		{
			"[]   戻る   []",
		},
	};
	char *PsaveData[6] =		//セーブデータ
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
	// セーブ画面作成
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

	//セーブデータの有無確認
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
		//キーボード操作処理
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
			//カーソルが重なってる場合
			if( key == CNTdata && ( INP(PM_CURX) >= 21 + 29 * (CNTdata % 3) || INP(PM_CURX) <= 2 + 29 * (CNTdata % 3)
				|| INP(PM_CURY) >= 11 + 11 * (CNTdata / 3) || INP(PM_CURY) <= 2 + 11 * (CNTdata / 3) )
				|| INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
			{
				//マウス操作の場合キーボード一時無効化
				if( INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
				{
					key = 13;
				}

				//選択された場合
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
						//カーソルがYESの上にある場合
						if( INP(PM_CURX) >= 25 && INP(PM_CURX) <= 33
							&& INP(PM_CURY) ==13)
						{
							COLOR(BLACK, CORAL);
							LOCATE(25, 13);
							for( x = 0; x < 9; x++ )
							{
								printf("%c",display[5][0][x]);
							}

							//クリックされた場合
							if( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 )
							{
								fp = fopen( PsaveData[CNTdata ], "wb" ); 
								if( fp == 0 )
								{
									printf("%cが開けませんでした。",PsaveData[CNTdata]);
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

						//カーソルがNOの上にある場合
						if( INP(PM_CURX) >= 45 && INP(PM_CURX) <= 53
							&& INP(PM_CURY) ==13)
						{
							//カーソルが上にある場合
							COLOR(BLACK, CORAL);
							LOCATE(45, 13);
							for( x = 0; x < 8; x++ )
							{
								printf("%c",display[6][0][x]);
							}

							//クリックされた場合
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

								//囲い生成
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
								
								//囲い内の文字生成 セーブデータがある場合
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

								//囲い内の文字生成 セーブデータがない場合
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

				//セーブデータがなかった場合
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] != 0 )
				{
					fp = fopen( PsaveData[CNTdata], "wb" ); 
					if( fp == 0 )
					{
						printf("%cが開けませんでした。",PsaveData[CNTdata]);
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

				//囲い生成
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
				
				//囲い内の文字生成 セーブデータがある場合
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

				//囲い内の文字生成 セーブデータがない場合
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

			//カーソルが重なってない場合
			else
			{
				COLOR(WHITE, BLACK);

				//囲い生成
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
				
				//囲い内の文字生成 セーブデータがある場合
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

				//囲い内の文字生成 セーブデータがない場合
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

		//ESCにカーソルが重なったら
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

			//クリックされた場合
			if( INP(PM_LEFT) != 0 || INP(PK_ESC) != 0 )
			{
				break;
			}
		}
		//ESCにカーソルが重なってなかったら
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
// 関数名 : load
// 返り値 : 0, 1
// 説明   : セーブデータのロード
////////////////////////////////////////////////////////////////////////////
int load (PLAYER *player)
{
	//関数宣言
	int x, y;		//AAの軸
	int key = 0;	//キーボード操作制御
	int CNTdata;	//セーブデータカウント
	int NUMdata[6];	//セーブデータの有無
	int END = 0;	//while文からの脱出
	char display[8][25][108] =	//セーブ画面AA
	{
		{
			"体力     : ",
			"攻撃     : ",
			"防御     : ",
			"すばやさ : ",
		},

		{
			"[ EMPTY ]" 
		},

		{
			"|￣￣￣￣￣￣￣￣￣|",
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
			"[]      　　　　　　　　　　　　　　　　　　　      []",
			"[]             セーブデータがありません             []",
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
			"[] ロードされました。[]", 
			"[]                   []",
		},

		{
			"[]   戻る   []",
		},
	};
	char *PsaveData[6] =	//セーブデータ
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
	// ロード画面作成
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

	//セーブデータの有無確認
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
		//キーボード操作処理
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
			//カーソルが重なってる場合
			if( key == CNTdata && ( INP(PM_CURX) >= 21 + 29 * (CNTdata % 3) || INP(PM_CURX) <= 2 + 29 * (CNTdata % 3)
				|| INP(PM_CURY) >= 11 + 11 * (CNTdata / 3) || INP(PM_CURY) <= 2 + 11 * (CNTdata / 3) )
				|| INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
			{
				//マウス操作の場合キーボード一時無効化
				if( INP(PM_CURX) >= 2 + 29 * (CNTdata % 3) && INP(PM_CURX) <= 21 + 29 * (CNTdata % 3)
				&& INP(PM_CURY) >= 2 + 11 * (CNTdata / 3) && INP(PM_CURY) <= 11 + 11 * (CNTdata / 3) )
				{
					key = 13;
				}

				//セーブデータがあった場合
				if( ( INP(PM_LEFT) != 0 || INP(PK_ENTER) != 0 ) && NUMdata[CNTdata] == 0 )
				{
					fp = fopen( PsaveData[CNTdata], "rb" );
					if(fp == 0)
					{
						printf("%cがロードされませんでした。",PsaveData[CNTdata]);
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

				//セーブデータがなかった場合
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

					//囲い生成
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
					
					//囲い内の文字生成 セーブデータがある場合
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

					//囲い内の文字生成 セーブデータがない場合
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

				//囲い生成
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
				
				//囲い内の文字生成 セーブデータがある場合
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

				//囲い内の文字生成 セーブデータがない場合
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

			//カーソルが重なってない場合
			else
			{
				COLOR(WHITE, BLACK);

				//囲い生成
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
				
				//囲い内の文字生成 セーブデータがある場合
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

				//囲い内の文字生成 セーブデータがない場合
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

		//ESCにカーソルが重なったら
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

			//クリックされた場合
			if( INP(PM_LEFT) != 0 || INP(PK_ESC) != 0 )
			{
				return 0;
			}
		}
		//ESCにカーソルが重なってなかったら
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
// 関数名 : battle
// 返り値 :
// 説明   : 君が泣くまで殴るのをやめないッ！！
////////////////////////////////////////////////////////////////////////////
int battleAnime (PLAYER *player)
{
	//関数宣言
	int x, y;		//AAのX座標、Y座標
	int CNTchar;	//for文の制御
	bool endBattle;	//戦いが終わったかどうか
	int turn;		//攻撃フェーズ決め
	int defoHP[2];	//デフォルトのHP
	int damage = 0;	//ダメージ
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
			"  ／            ＼  ",
			" ::               ` ",
			"::::               |",
			":::::              |",
			":::::.             |",
		},

		{
			"    ___________     ",
			"   /           `    ",
			"  |   ||   ||   |   ",
			"  |     ー      |   ",
			"|＼ . ...      /    ",
			" ＼＼        :      ",
			"   ＼＼／    |      ",
			"    :__       :     ",
			"    |_______ __:    ",
			"     ||     ||      ",
			" ____ ||_____||_____",
		},

		{
			"￣￣￣￣￣￣|",
			"            |",
			"____________|",
		},

		{
			"|￣￣￣￣￣￣",
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
			"／  BATTLE LOG  ",
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
		"は遠い目をしている",
		"は余裕そうな顔をしている",
		"はつぶらな瞳で見つめている",
		"「I want to win the beatbox battle」",

	};
	srand((unsigned) time (NULL));		//乱数の種まき

	//-------------------------------
	// 戦闘画面作成
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
		printf("￣");
	}

	for( y = 0; y < 2; y++ )
	{
		LOCATE(62, 19 + y);
		for( x = 0; x < 16; x++ )
		{
			printf("%c",display[7][y][x]);
		}
	}

	//キャラクター名
	for( y = 0; y < 3; y++ )
	{
		//プレイヤー
		LOCATE(3, 3 + y);
		for( x = 0; x < 13; x++ )
		{
			printf("%c",display[3][y][x]);
		}

		//敵
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

	//敵出現
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

	//敵移動
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

	//プレイヤー出現
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

	//プレイヤー移動
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

	//クリックかEnter待ち
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
	//バトル処理
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
		//BATTLE LOG初期化
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
	// バトルエンド処理
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
// 関数名 : battle
// 返り値 : 
// 説明   : 戦が始まる
////////////////////////////////////////////////////////////////////////////
bool battle ( PLAYER *ATCplayer, PLAYER *DEFplayer )
{
	//関数宣言
	int avoidance;		//回避率
	int damage;			//ダメージ
	int next;			//クリックかEnter待ち

	srand( ( unsigned )time( NULL ) );		//ランダム

	//-------------------------------
	// 戦闘処理
	//-------------------------------
	avoidance = rand();
	
	/*if( ( avoidance / 600 ) * DEFplayer->Speed < ( rand() * rand() / 2 ) % avoidance )
	{
		printf("%sの攻撃は回避された",ATCplayer->Name);
		WAIT(700);
		return false;
	}*/
	//else
	//{
		damage = ATCplayer->Attack - ( ATCplayer->Attack * ( DEFplayer->Def / 600 ) );
		LOCATE(3, 22);
		printf("%sの攻撃",ATCplayer->Name);
		WAIT(600);
		LOCATE(3, 23);
		printf("%sは%dのダメージを受けた",DEFplayer->Name ,damage);

		DEFplayer->Life -= ATCplayer->Attack;
		if( DEFplayer->Life < 0 )
		{
			DEFplayer->Life = 0;
		}

		//クリックかEnter待ち
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