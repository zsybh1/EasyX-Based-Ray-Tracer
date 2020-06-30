#include <graphics.h>			
#include <iostream>
#include <cmath>
#include <easyxmath.h>
#include <easyxobject.h>
#include <typeinfo>
#include <vector>
#include <thread>
using namespace std;

class scene {
public:
	float dist;
	pos3 e;
	vec3 v;
	vec3 u;
	vec3 w;
	unsigned int lightNum;
	pos3 *lightList;
	unsigned int objectNum;
	surface **objectList;
	int HEIGHT, WIDTH;
};

scene init();
void render(const scene &s, int linein, int lineout);
color RayTrace(const scene &s, const ray &l, const color &lightColor, int layer);
int limits = 10;

int main() {
	const scene &Scene = init();

	//四线程绘图
	thread thread1(render, Scene, Scene.HEIGHT / 2, Scene.HEIGHT / 4);
	thread thread2(render, Scene, Scene.HEIGHT / 4, 0);
	thread thread3(render, Scene, 0, -Scene.HEIGHT / 4);
	render(Scene, -Scene.HEIGHT / 4, -Scene.HEIGHT / 2);
	thread1.join();
	thread2.join();
	thread3.join();

	saveimage(L"test.png");
	getchar();
	closegraph();
}


scene init() {
	// 窗口设定
	int HEIGHT = 480  ;
	int WIDTH = 800 ;
	initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE);
	setorigin(WIDTH / 2, HEIGHT / 2);
	setaspectratio(1, -1);

	// 视点设定
	float dist = HEIGHT * 2.0f;
	pos3 e(0.0, 0.0, -dist );
	vec3 v(0.0, 1.0, 0.0);
	vec3 u(1.0, 0.0, 0.0);
	vec3 w(0.0, 0.0, -1.0);

	float up = 2000.0f;
	float down = -200.0f;
	float left = -600.0f;
	float right = 600.0f;
	float front = 1000.0f;
	float back = -dist-100.0f;

	//灯光设定
	unsigned int lightNum = 1;
	//	pos light1(-200.0, 400.0, 50.0);
	//pos light2(-200.0, 0.0, 0.0);
	pos3* lightList = new pos3[lightNum];
	lightList[0] = pos3(0.0, up - 300.0f, back / 2);

	// 物体设定
	unsigned objectNum = 15;
	colored_sphere s1(pos3(-300.0, 0.0, 200.0), 200.0, color(BGR(RED)));
	colored_sphere s2(pos3(0.0, -100.0, 500.0), 100.0, color(255, 128, 64));
	colored_sphere s3(pos3(300.0, 0.0, 200.0), 200.0, color(BGR(YELLOW)));
	colored_triangle t1(pos3(right, down, back), pos3(right, down, front), pos3(left, down, front), color(BGR(WHITE)));
	colored_triangle t2(pos3(right, down, back), pos3(left, down, back), pos3(left, down, front), color(BGR(WHITE)));
	colored_triangle t3(pos3(left, down, front), pos3(right, down, front), pos3(right, up, front), color(BGR(WHITE)));
	colored_triangle t4(pos3(left, down, front), pos3(left, up, front), pos3(right, up, front), color(BGR(WHITE)));
	colored_triangle t5(pos3(left, down, back), pos3(left, up, back), pos3(left, up, front), color(BGR(WHITE)));
	colored_triangle t6(pos3(left, down, back), pos3(left, down, front), pos3(left, up, front), color(BGR(WHITE)));
	colored_triangle t7(pos3(right, down, back), pos3(right, up, back), pos3(right, up, front), color(BGR(WHITE)));
	colored_triangle t8(pos3(right, down, back), pos3(right, down, front), pos3(right, up, front), color(BGR(WHITE)));
	colored_triangle t9(pos3(left, down, back), pos3(right, down, back), pos3(right, up, back), color(BGR(WHITE)));
	colored_triangle t10(pos3(left, down, back), pos3(left, up, back), pos3(right, up, back), color(BGR(WHITE)));
	colored_triangle t11(pos3(right, up, back), pos3(right, up, front), pos3(left, up, front), color(BGR(WHITE)));
	colored_triangle t12(pos3(right, up, back), pos3(left, up, back), pos3(left, up, front), color(BGR(WHITE)));
	surface** objectList = new surface* [objectNum];
	objectList[0] = new colored_sphere(s1);
	objectList[1] = new colored_sphere(s2);
	objectList[2] = new colored_sphere(s3);
	objectList[3] = new colored_triangle(t1);
	objectList[4] = new colored_triangle(t2);
	objectList[5] = new colored_triangle(t3);
	objectList[6] = new colored_triangle(t4);
	objectList[7] = new colored_triangle(t5);
	objectList[8] = new colored_triangle(t6);
	objectList[9] = new colored_triangle(t7);
	objectList[10] = new colored_triangle(t8);
	objectList[11] = new colored_triangle(t9);
	objectList[12] = new colored_triangle(t10);
	objectList[13] = new colored_triangle(t11);
	objectList[14] = new colored_triangle(t12);

	return scene{ dist,e,v,u,w,lightNum,lightList,objectNum,objectList, HEIGHT, WIDTH };
}

void render(const scene &s, int linein, int lineout) {
	for (int j = linein; j > lineout; --j) {
		for (int i = -s.WIDTH / 2; i < s.WIDTH / 2; ++i) {
			// 生成光线
			vec3 d = -s.dist * s.w + (float)i * s.u + (float)j * s.v;
			ray l(s.e, d);

			putpixel(i + s.WIDTH / 2, j + s.HEIGHT / 2, RayTrace(s, l, color(0xFFFFFF), 0).getBGR());
		}
	}
}

color RayTrace(const scene &s, const ray &l, const color &lightColor, int layer) {
	if (layer >= limits)return lightColor;
	unsigned int objectNum = s.objectNum;
	surface **objectList = s.objectList;
	unsigned int lightNum = s.lightNum;
	pos3 *lightList = s.lightList;
	color ret;
	// 初始化深度值
	float t0 = 10000.0;
	float t;
	surface *hitObject = nullptr;
	// 对每个物体求交
	for (unsigned int k = 0; k < objectNum; ++k) {
		if (objectList[k]->intersect_ray(l, t) && t < t0) {
			hitObject = objectList[k];
			// 更新深度值
			t0 = t;
		}
	}

	if (hitObject != nullptr && (typeid(*hitObject) == typeid(colored_sphere) || typeid(*hitObject) == typeid(colored_triangle))) {
		colored_surface &ob = *dynamic_cast<colored_surface *>(hitObject);
		color result;
		pos3 p = l.e + t0 * 0.9999  * l.d;
		vec3 n = hitObject->normal(p);
		vec3 viewDir = (-l.d).normalize();
		// 环境光
		result += 0.2f * ob.rgb;
		float power = 100;

		for (unsigned int li = 0; li < lightNum; ++li) {
			vec3 lightLine = lightList[li] - p;
			vec3 lightDir = lightLine.normalize();
			ray lightRay(p, lightLine);
			vec3 halfDir = (viewDir + lightDir).normalize();
			float dis;
			bool flag = true;
			for (unsigned int ki = 0; ki < objectNum; ++ki) {
				if (objectList[ki] == hitObject) {
					continue;
				}
				if (objectList[ki]->intersect_ray(lightRay, dis) && dis <= 1) {
					//被物体遮挡
					flag = false;
					break;
				}
			}
			if (flag) {
			//	result += (0.5f * max(0.0f, dot(n, lightDir)) * ob.rgb + 1.0f * pow(max(0.0f, dot(n, halfDir)), power) * ob.rgb)
			//		* vec3(lightColor.R / 0x100, lightColor.G / 0x100, lightColor.B / 0x100);
				result += (0.5f * max(0.0f, dot(n, lightDir)) * ob.rgb)
					* vec3(lightColor.R / 0x100, lightColor.G / 0x100, lightColor.B / 0x100);
			}
			vec3 reflectDir = -viewDir + 2.0 * dot(viewDir, n) * n;
			result += 0.5 * RayTrace(s, ray(p, reflectDir), result, layer + 1);
		}
		
		ret = result;


	}
	return ret;
}
