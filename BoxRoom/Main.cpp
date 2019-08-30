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
	Point lookFrom(0.0f, 0.0f, -7.0f);
	Point lookAt(0.0f, 0.0f, 0.0f);
	Vector up(0.0f, 1.0f, 0.0f);
	float fous = 10.0f;
	float aperture = 0.0f;
	float vfov = 45.0f;
	Camera camera(lookFrom, lookAt, up, vfov, aspect, aperture, fous, 0.0f, 1.0f);

	auto white = make_shared<ConstantTexture>(Color(0.73f, 0.73f, 0.73f));
	auto red = make_shared<ConstantTexture>(Color(0.65f, 0.05f, 0.05f));
	auto green = make_shared<ConstantTexture>(Color(0.15f, 0.45f, 0.15f));
	auto blue = make_shared<ConstantTexture>(Color(0.12f, 0.12f, 0.45f));
	auto light = make_shared<ConstantTexture>(Color(10.0f, 10.0f, 10.0f));

	vector<unique_ptr<Hitable>> hitables;
	hitables.emplace_back(make_unique<FlipNormal>(make_unique<YZRect>(-2.0f, 2.0f, -5.0f, 2.0f, -2.0f, make_shared<Lambertian>(green))));
	hitables.emplace_back(make_unique<XYRect>(-2.0f, 2.0f, -2.0f, 2.0f, 2.0f, make_shared<Lambertian>(blue)));
	hitables.emplace_back(make_unique<YZRect>(-2.0f, 2.0f, -5.0f, 2.0f, 2.0f, make_shared<Lambertian>(red)));
	hitables.emplace_back(make_unique<FlipNormal>(make_unique<XZRect>(-2.0f, 2.0f, -5.0f, 2.0f, -2.0f, make_shared<Lambertian>(white))));
	hitables.emplace_back(make_unique<XZRect>(-2.0f, 2.0f, -5.0f, 2.0f, 2.0f, make_shared<Lambertian>(white)));
	hitables.emplace_back(make_unique<XZRect>(-0.75f, 0.75f, -0.75f, 0.75f, 1.99f, make_shared<DiffuseLight>(light)));
	hitables.emplace_back(make_unique<Sphere>(Point(1.1f, -1.25f, -1.1f), 0.75f, make_shared<Dielectric>(1.5f)));
	hitables.emplace_back(make_unique<Sphere>(Point(-1.0f, -1.25f, 0.5f), 0.75f, make_shared<Metal>(Color(1.0f, 1.0f, 1.0f), 0.0f)));
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