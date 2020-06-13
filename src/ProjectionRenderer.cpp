#include <graphics.h>			// 引用图形库头文件
#include <iostream>
#include <cmath>
#include <easyxmath.h>
#include <easyxobject.h>
#include <typeinfo>
using namespace std;

int main() {

	//===========================
	//== 初始化
	//===========================

	// 窗口设定
	const int HEIGHT = 480 * 2, WIDTH = 640 * 2;
	initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE);
	setorigin(WIDTH / 2, HEIGHT / 2);
	setaspectratio(1, -1);

	// 视点设定
	float dist = 4.0f;
	pos e(0.0, 0.0, -dist);
	vec3 v(0.0, 1.0, 0.0);
	vec3 u(1.0, 0.0, 0.0);
	vec3 w(0.0, 0.0, -1.0);
    
	//灯光设定
	unsigned int lightNum = 2;
	pos light1(0.0, 400.0, 50.0);
	pos light2(300.0, 400.0, 0.0);
	pos lightList[] = { light1, light2 };

	// 物体设定
	unsigned int objectNum = 1;
	triangle t1(pos(1.0, 1.0, 1.0), pos(2.0, 0.0, 1.0), pos(2.0, 1.0, 1.0));
	triangle *objectList[] = {&t1};
	
	//============================
	//== 遍历物体进行渲染
	//============================
	
	for (int i = 0; i < objectNum; ++i) {
		triangle &tri = *objectList[i];
		pos *r[3] = { &tri.A,&tri.B,&tri.C};
		for (int j = 0; j < 3; ++j) {
			float t = -r[j]->z / e.z;
			float x = e.x + t * r[j]->x;
			float y = e.y + t * r[j]->y;
			int x0 = x * 200;
			int y0 = y * 200;
			putpixel(x0, y0, WHITE);
		}
	}
	saveimage(L"test.png");
	getchar();
	
	//==============================
	//== 善后
	//==============================

	closegraph();
	return 0;
}

