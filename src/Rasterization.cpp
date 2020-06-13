#include <graphics.h>			// 引用图形库头文件
#include <iostream>
#include <cmath>
#include <easyxmath.h>
#include <easyxobject.h>
#include <typeinfo>
using namespace std;

void draw_tricolor_triangle(const tricolor_triangle &tri, const pos &e);

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
	float dist = 3.0f;
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
	tricolor_triangle t1(pos(-1.0, -1.0, 1.0), pos(2.5, -2.0, 1.0), pos(2.0, 2.0, 1.0), BGR(RED), BGR(GREEN), BGR(BLUE));
	tricolor_triangle *objectList[] = {&t1};
	
	//============================
	//== 遍历物体进行渲染
	//============================
	for (unsigned k = 0; k < objectNum; ++k) {
		draw_tricolor_triangle(*objectList[k], e);
	}
	saveimage(L"test.png");
	getchar();
	
	//==============================
	//== 善后
	//==============================

	closegraph();
	return 0;
}

void draw_tricolor_triangle(const tricolor_triangle &tri, const pos &e) {
	const pos *r[3] = { &tri.A,&tri.B,&tri.C };
	pos p[3];
	for (int j = 0; j < 3; ++j) {
		float t = -r[j]->z / e.z;
		float x = e.x + t * r[j]->x;
		float y = e.y + t * r[j]->y;
		p[j].x = int(x * 500);
		p[j].y = int(y * 500);
	}
	int maxX = max(max(p[0].x, p[1].x), p[2].x);
	int maxY = max(max(p[0].y, p[1].y), p[2].y);
	int minX = min(min(p[0].x, p[1].x), p[2].x);
	int minY = min(min(p[0].y, p[1].y), p[2].y);
	vec3 vec[3] = { vec3(),p[1] - p[0], p[2] - p[0] };
	for (int i = maxY; i >= minY; --i) {
		for (int j = minX; j <= maxX; ++j) {
			vec[0] = pos(j, i, 0) - p[0];
			float M = vec[1].x * vec[2].y - vec[2].x * vec[1].y;
			float u = (vec[1].x * vec[0].y - vec[0].x * vec[1].y) / M;
			float v = -(vec[2].x * vec[0].y - vec[0].x * vec[2].y) / M;
			if (0 <= u && u <= 1 && 0 <= v && v <= 1 - u) {
				color Color = tri.Acolor * (1 - u - v) + tri.Bcolor * v + tri.Ccolor * u;
				putpixel(j, i, Color.getBGR());
			}
		}
	}
}