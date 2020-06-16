#include <graphics.h>			// 引用图形库头文件
#include <iostream>
#include <cmath>
#include <easyxmath.h>
#include <easyxobject.h>
#include <typeinfo>
using namespace std;

color RayTrace(const ray &l, surface *objectList[], unsigned int objectNum, pos lightList[], unsigned int lightNum, const color &lightColor);
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
	float dist = 480.0 * 3.0;
	pos e(0.0, 0.0, -dist);
	vec3 v(0.0, 1.0, 0.0);
	vec3 u(1.0, 0.0, 0.0);
	vec3 w(0.0, 0.0, -1.0);
    
	//灯光设定
	unsigned int lightNum = 1;
//	pos light1(-200.0, 400.0, 50.0);
	pos light2(-200.0, 0.0, 0.0);
	pos lightList[] = { light2 };

	// 物体设定
	unsigned int objectNum = 3;
	colored_sphere s1(pos(-200.0, 200.0, 200.0), 200.0, color(BGR(RED)));
	colored_sphere s2(pos(100.0, 100.0, 100.0), 100.0, color(255, 128, 64));
	colored_sphere s3(pos(0.0, -250.0, 200.0), 200.0, color(WHITE));
	//colored_triangle t1(pos(-1000.0, -HEIGHT / 2, 0.0), pos(1000.0, -HEIGHT / 2, 0.00), pos(4000.0, -HEIGHT / 2, 4000.0), color(BGR(WHITE)));
	//colored_triangle t2(pos(-1000.0, -HEIGHT / 2, 0.0), pos(-4000.0, -HEIGHT / 2, 4000.00), pos(4000.0, -HEIGHT / 2, 4000.0), color(BGR(WHITE)));
	surface *objectList[] = { &s1, &s2, &s3};
	
	//============================
	//== 遍历像素进行渲染
	//============================
	
	for (int j = HEIGHT / 2; j > -HEIGHT / 2; --j) {
		for (int i = -WIDTH / 2; i < WIDTH / 2; ++i) {
			// 生成光线
			vec3 d = -dist * w + (float)i * u + (float)j * v;
			ray l(e, d);

			putpixel(i, j, RayTrace(l, objectList, objectNum, lightList, lightNum, color(0xFFFFFF)).getBGR());
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

color RayTrace(const ray &l, surface *objectList[], unsigned int objectNum, pos lightList[], unsigned int lightNum, const color &lightColor) {
	color ret;
	// 初始化深度值
	float t0 = 10000.0;
	float t;

	// 对每个物体求交
	for (unsigned int k = 0; k < objectNum; ++k) {
		// 通过深度测试则绘制像素点
		if (objectList[k]->intersect_ray(l, t) && t < t0) {
			if (typeid(*objectList[k]) == typeid(colored_sphere) || typeid(*objectList[k]) == typeid(colored_triangle)) {
				colored_surface &ob = *dynamic_cast<colored_surface *>(objectList[k]);
				color result;
				pos p = l.e + t * l.d;
				vec3 n = objectList[k]->normal(p - l.d.normalize() * 0.01);
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
						if (ki == k) continue;
						if (objectList[ki]->intersect_ray(lightRay, dis) && dis <= 1) {
							//被物体遮挡
							flag = false;
							break;
						}
					}
					if (flag) {
						result += (0.5f * max(0.0f, dot(n, lightDir)) * ob.rgb + 1.0f * pow(max(0.0f, dot(n, halfDir)), power) * ob.rgb)
							*vec3(lightColor.R/0x100, lightColor.G/0x100, lightColor.B/0x100);
						vec3 reflectDir = 2 * n - viewDir * sqrt(2.0f);
						if (result.R>0.1 || result.G>0.1|| result.B>0.1)
							result +=  0.5*RayTrace(ray(p, reflectDir), objectList, objectNum, lightList, lightNum, result);
					}
				}

				ret = result;

				// 更新深度值
				t0 = t;
			}
		}
	}

	return ret;
}