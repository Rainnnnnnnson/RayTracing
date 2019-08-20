#include "../RayTracing/Math.h"
#include "../RayTracing/Geometry.h"
#include "../RayTracing/RGBImage.h"
#include "../RayTracing/Camera.h"
#include "../RayTracing/Application.h"
#include "../RayTracing/Instance.h"
#include "../RayTracing/Special.h"
#include <cmath>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <chrono>
using std::vector;
using std::mutex;
using std::thread;
using std::atomic;

Color GetPixelColor(Ray ray, Hitable& hits, int depth);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					_In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	int width = 800;
	int height = 600;
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	Application application(width, height);

	/*
		���̷߳Ƕ�����ѯ������RGBImage
		�޸Ķ���߳������Լ�����ɫ����ۼ���colorImage��
		�޸�caculateCount��colorImage ��Ҫ����
		ÿ���̼߳���һ����
		1.   caculateCount += 1
		2.   colorImageÿ�������ۼ���ɫֵ

	*/
	mutex imageLock;
	int caculateCount = 0;
	Texture2D<Color> colorImage(width, height);

	/*
		mainCaculateCount�������Ӧ
		�������̲߳����Ƿ���Ҫˢ��ͼƬ
	*/
	int mainCaculateCount = 0;
	RGBImage image(width, height);
	application.CopyInBuffer(image);
	application.Continue();

	/*
		�˳�ʱ��Ҫ�޸����
		��֤�����߳�������ȫ��һ���˳�
	*/
	atomic<bool> exit(false);

	/*
		�����������
	*/
	Point lookFrom(-250.0f, 250.0f, -750.0f);
	Point lookAt(250.0f, 250.0f, 250.0f);
	Vector up(0.0f, 1.0f, 0.0f);
	float fous = 1000.0f;
	float aperture = 1.0f;
	float vfov = 45.0f;
	Camera camera(lookFrom, lookAt, up, vfov, aspect, aperture, fous, 0.0f, 1.0f);

	/*
		һ��ʼ�������Hitable������
	*/
	vector<unique_ptr<Hitable>> hitables;

	/*
		���еذ��ŵ�һ��
		������һ��BVHTreeװ
	*/
	vector<unique_ptr<Hitable>> someBoxs;
	auto ground = make_shared<Lambertian>(make_shared<ConstantTexture>(Color(0.48f, 0.83f, 0.53f)));
	int count = 10;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			const float w = 100.0f;
			float x0 = -250.0f + static_cast<float>(i) * w;
			float z0 = -250.0f + static_cast<float>(j) * w;
			float x1 = x0 + w;
			float z1 = z0 + w;
			float y0 = -0.01f;
			float y1 = 100.0f * Random();
			someBoxs.emplace_back(make_unique<Box>(Point(x0, y0, z0), Point(x1, y1, z1), ground));
		}
	}
	hitables.emplace_back(make_unique<BVHTree>(std::move(someBoxs), 0.0f, 1.0f));

	/*
		�����ĵƹ�
	*/
	auto light = make_shared<DiffuseLight>(make_shared<ConstantTexture>(Color(7.0f, 7.0f, 7.0f)));
	hitables.emplace_back(make_unique<XZRect>(100.0f, 400.0f, 100.0f, 400.0f, 550.0f, light));

	/*
		�ƶ�����
	*/
	Point moveFrom(400.0f,400.0f,200.0f);
	Point moveTo(450.0f, 400.0f, 200.0f);
	auto moveMaterial = make_shared<Lambertian>(make_shared<ConstantTexture>(Color(0.7f, 0.3f, 0.1f)));
	hitables.emplace_back(make_unique<MovingSphere>(moveFrom, moveTo, 0.0f, 1.0f, 50.0f, moveMaterial));

	/*
		������
	*/
	auto dielectric = make_shared<Dielectric>(1.5f);
	hitables.emplace_back(make_unique<Sphere>(Point(100.0f, 150.0f, 45.0f), 50.0f, dielectric));

	/*
		������
	*/
	auto metal = make_shared<Metal>(Color(0.8f, 0.8f, 0.9f), 10.0f);
	hitables.emplace_back(make_unique<Sphere>(Point(0.0f, 150.0f, 250.0f), 50.0f, metal));

	/*
		�����������������
		Ч������ɫ����������в���Ч��
	*/
	Point sphereCenter = Point(300.0f, 270.0f, -50.0f);
	float sphereR = 70.0f;
	auto sphere1 = make_unique<Sphere>(sphereCenter, sphereR, make_shared<Dielectric>(1.5f));
	hitables.emplace_back(std::move(sphere1));
	auto sphere2 = make_unique<Sphere>(sphereCenter, sphereR, nullptr);
	hitables.emplace_back(make_unique<ConstantMedium>(std::move(sphere2), 0.2f, make_shared<ConstantTexture>(Color(0.2f, 0.4f, 0.9f))));

	/*
		������Χ��һ���������
	*/
	auto boundary = make_unique<Sphere>(Point(0.0f, 0.0f, 0.0f), 5000.0f, nullptr);
	hitables.emplace_back(make_unique<ConstantMedium>(std::move(boundary), 0.0001f, make_shared<ConstantTexture>(Color(1.0f, 1.0f, 1.0f))));

	/*
		����
	*/
	auto earthImage = application.GetImage(L"../earth.jpg");
	hitables.emplace_back(make_unique<CircleTextureSphere>(Point(400.0f, 200.0f, 400.0f), 100.0f, std::move(earthImage)));

	/*
		������
	*/
	auto noise = make_shared<Lambertian>(make_shared<TurbulenceTexture>(0.1f, 0.1f, 5.0f, 7.0f));
	hitables.emplace_back(make_unique<Sphere>(Point(220.0f, 350.0f, 400.0f), 80.0f, noise));

	/*
		�ܶ���
	*/
	vector<unique_ptr<Hitable>> someSphere;
	auto white = make_shared<Lambertian>(make_shared<ConstantTexture>(Color(0.73f, 0.73f, 0.73f)));
	for (int k = 0; k < 1000; k++) {
		Point center(165.0f * Random(), 165.0f * Random(), 165.0f * Random());
		someSphere.emplace_back(make_unique<Sphere>(center, 10.0f, white));
	}
	Vector direction(-100.0f, 270.0f, 395.0f);
	hitables.emplace_back(make_unique<Translate>(make_unique<RotateYAxis>(make_unique<BVHTree>(std::move(someSphere), 0.0f, 1.0f), 15.0f), direction));

	auto hit = BVHTree(std::move(hitables), 0.0f, 1.0f);

	/*
		����ÿ���̵߳�����
		������һ����
	*/
	auto threadTask = [&]() {
		Texture2D<Color> threadSingleImage(width, height);
		while (!exit) {
			//����ͼƬ
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					float u = (static_cast<float>(i) + Random()) / static_cast<float>(width);
					float v = (static_cast<float>(j) + Random()) / static_cast<float>(height);
					Ray ray = camera.GetRay(u, v);
					Color color = GetPixelColor(ray, hit, 50);
					threadSingleImage.SetScreenPoint(i, j, color);
				}
			}
			{
				std::lock_guard<mutex> lock(imageLock);
				caculateCount += 1;
				//���õ�����ֵ�ۼ������߳���
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						Color singleColor = threadSingleImage.GetImagePoint(x, y);
						Color newColor = colorImage.GetImagePoint(x, y) + singleColor;
						colorImage.SetImagePoint(x, y, newColor);
					}
				}
			}
		}
	};
	/*
		�߳̿�ʼ
		�ҵĵ���ֻ���ĸ��߳�
	*/
	constexpr int threadCount = 4;
	vector<thread> threadPool(threadCount);
	for (auto& t : threadPool) {
		t = thread(threadTask);
	}

	/*
		���߳���ѯ
	*/

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while (application.Continue()) {
		{
			std::lock_guard<mutex> lock(imageLock);
			if (mainCaculateCount != caculateCount) {
				mainCaculateCount = caculateCount;
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						Color newColor = colorImage.GetImagePoint(x, y) / static_cast<float>(mainCaculateCount);
						image.SetImagePoint(x, y, newColor);
					}
				}
				application.CopyInBuffer(image);

			}
			/*
				window���� ��ʾ �������� �� ����ʱ��
			*/
			auto delta = std::chrono::steady_clock::now() - start;
			std::wstring s;
			s.reserve(50);
			s.append(L"RayTracing   RayCount : ");
			s.append(std::to_wstring(mainCaculateCount));
			s.append(L"   TotalTime : ");
			auto t1 = std::chrono::high_resolution_clock::now();
			auto seconds = (std::chrono::duration_cast<std::chrono::seconds>(delta) % 60).count();
			auto minutes = (std::chrono::duration_cast<std::chrono::minutes>(delta) % 60).count();
			auto hours = std::chrono::duration_cast<std::chrono::hours>(delta).count();
			//Сʱ
			if (hours < 10) {
				s.append(L"0");
			}
			s.append(std::to_wstring(hours));
			//����
			s.append(L":");
			if (minutes < 10) {
				s.append(L"0");
			}
			s.append(std::to_wstring(minutes));
			//��
			s.append(L":");
			if (seconds < 10) {
				s.append(L"0");
			}
			s.append(std::to_wstring(seconds));
			application.SetWindowsText(s.c_str());
		}
		std::this_thread::yield();
	}

	/*
		�����˳����ȴ�
	*/
	exit = true;
	for (auto& t : threadPool) {
		t.join();
	}
	return 0;
}

Color GetPixelColor(Ray ray, Hitable& hits, int depth) {
	HitRecord record;
	if (!hits.Hit(ray, 0.001f, FLT_MAX, record)) {
		return Color(0.0f, 0.0f, 0.0f);
	}

	Ray scattered;
	Color emitted;
	Color attenuation;
	record.hitable->Calculate(record, scattered, emitted, attenuation);
	if (depth >= 0) {
		return emitted + attenuation * GetPixelColor(scattered, hits, depth - 1);
	} else {
		return emitted;
	}
}