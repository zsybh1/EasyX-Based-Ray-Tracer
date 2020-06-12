#include <graphics.h>			// ����ͼ�ο�ͷ�ļ�
#include <iostream>
#include <cmath>
#include <easyxmath.h>
#include <easyxobject.h>
#include <typeinfo>
using namespace std;

int main() {

	//===========================
	//== ��ʼ��
	//===========================

	// �����趨
	const int HEIGHT = 480 * 2, WIDTH = 640 * 2;
	initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE);
	setorigin(WIDTH / 2, HEIGHT / 2);
	setaspectratio(1, -1);

	// �ӵ��趨
	float dist = 480.0 * 3.0;
	pos e(0.0, 0.0, -dist);
	vec3 v(0.0, 1.0, 0.0);
	vec3 u(1.0, 0.0, 0.0);
	vec3 w(0.0, 0.0, -1.0);
    
	//�ƹ��趨
	unsigned int lightNum = 2;
	pos light1(0.0, 400.0, 50.0);
	pos light2(300.0, 400.0, 0.0);
	pos lightList[] = { light1, light2 };

	// �����趨
	unsigned int objectNum = 5;
	colored_sphere s1(pos(0.0, 200.0, 200.0), 50.0, color(BGR(RED)));
	colored_sphere s2(pos(0.0, 50.0, 200.0), 100.0, color(255, 128, 64));
	colored_sphere s3(pos(0.0, -250.0, 200.0), 200.0, color(BGR(GREEN)));
	colored_triangle t1(pos(-1000.0, -HEIGHT / 2, 0.0), pos(1000.0, -HEIGHT / 2, 0.00), pos(4000.0, -HEIGHT / 2, 4000.0), color(BGR(BLUE)));
	colored_triangle t2(pos(-1000.0, -HEIGHT / 2, 0.0), pos(-4000.0, -HEIGHT / 2, 4000.00), pos(4000.0, -HEIGHT / 2, 4000.0), color(BGR(BLUE)));
	surface *objectList[] = { &s1, &s2, &s3, &t1, &t2};
	
	//============================
	//== �������ؽ�����Ⱦ
	//============================
	
	for (int j = HEIGHT / 2; j > -HEIGHT / 2; --j) {
		for (int i = -WIDTH / 2; i < WIDTH / 2; ++i) {
			// ���ɹ���
			vec3 d = -dist * w + (float)i * u + (float)j * v;
			ray l(e, d);

			// ��ʼ�����ֵ
			float t0 = 10000.0;
			float t;

			// ��ÿ��������
			for (unsigned int k = 0; k < objectNum; ++k) {
				// ͨ����Ȳ�����������ص�
				if (objectList[k]->intersect_ray(l, t) && t < t0) {
					if (typeid(*objectList[k]) == typeid(colored_sphere) || typeid(*objectList[k]) == typeid(colored_triangle)) {
						colored_surface &ob = *dynamic_cast<colored_surface *>(objectList[k]);
						color result;
						pos p = l.e + t * l.d;
						vec3 n = objectList[k]->normal(p - d.normalize() * 0.01);
						vec3 viewDir = (-l.d).normalize();
						// ������
						result += 0.2f * ob.rgb;
						float power = 100;

						for (unsigned int li = 0; li < lightNum ; ++li) {
							vec3 lightLine = lightList[li] - p;
							vec3 lightDir = lightLine.normalize();
							ray lightRay(p, lightLine);
							vec3 halfDir = (viewDir + lightDir).normalize();
							float dis;
							bool flag = true;
							for (unsigned int ki = 0; ki < objectNum; ++ki) {
								if (ki == k) continue;
								if (objectList[ki]->intersect_ray(lightRay, dis) && dis <= 1) {
									flag = false;
									break;
								}
							}
							if (flag)
								result += 0.5f * max(0.0f, dot(n, lightDir)) * ob.rgb + 1.0f * pow(max(0.0f, dot(n, halfDir)), power) * ob.rgb;
						}

						putpixel(i, j, result.getBGR());

						// �������ֵ
						t0 = t;
					}
				}
			}
		}
	}
	saveimage(L"test.png");
	getchar();
	
	//==============================
	//== �ƺ�
	//==============================

	closegraph();
	return 0;
}

