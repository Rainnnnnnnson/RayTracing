#include "Math.h"
#include "Geometry.h"
#include "RGBImage.h"
#include "Camera.h"
#include "Application.h"
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
		主线程非堵塞轮询将其变成RGBImage
		修改多个线程运算自己的颜色最后累加至colorImage中
		修改caculateCount和colorImage 需要加锁
		每个线程计算一次则
		1.   caculateCount += 1
		2.   colorImage每个像素累加颜色值

	*/
	mutex imageLock;
	int caculateCount = 0;
	Texture2D<Color> colorImage(width, height);

	/*
		mainCaculateCount与上面对应
		用于主线程测试是否需要刷新图片
	*/
	int mainCaculateCount = 0;
	RGBImage image(width, height);
	application.CopyInBuffer(image);
	application.Continue();

	/*
		退出时需要修改这个
		保证其他线程运行完全部一起退出
	*/
	atomic<bool> exit(false);

	/*
		设置相机参数
	*/
	Point lookFrom(0.0f, 1.0f, -4.0f);
	Point lookAt(0.0f, 0.0f, 0.0f);
	float fous = 10.0f;
	float aperture = 0.0f;
	float vfov = 45.0f;
	Camera camera(lookFrom, lookAt, Vector(0.0f, 1.0f, 0.0f), vfov, aspect, aperture, fous, 0.0f, 1.0f);
	
	/*
		设置场景
	*/
	shared_ptr<Lambertian> lambertian = make_shared<Lambertian>(make_unique<ConstantTexture>(Color(0.5f, 0.5f, 0.5f)));

	vector<unique_ptr<Hitable>> hitables;
	hitables.emplace_back(make_unique<Sphere>(Point(0.0f, 0.0f, 1.0f), 0.5f, lambertian));
	hitables.emplace_back(make_unique<Sphere>(Point(0.0f, -100.5f, 1.0f), 100.0f, lambertian));

	auto hitList = HitList(std::move(hitables));
	//BVHTree bvhTree(std::move(hitables), 0.0f, 1.0f);

	/*
		布置每个线程的任务
		任务都是一样的
	*/
	auto threadTask = [&]() {
		Texture2D<Color> threadSingleImage(width, height);
		while (!exit) {
			//计算图片
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					float u = (static_cast<float>(i) + Random()) / static_cast<float>(width);
					float v = (static_cast<float>(j) + Random()) / static_cast<float>(height);
					Ray ray = camera.GetRay(u, v);
					Color color = GetPixelColor(ray, hitList, 50);
					threadSingleImage.SetScreenPoint(i, j, color);
				}
			}
			{
				std::lock_guard<mutex> lock(imageLock);
				caculateCount += 1;
				//将得到的数值累加至主线程中
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						Color singleColor = threadSingleImage.GetImagePoint(x, y);
						/*
							需要将颜色范围压缩至[0,1]
						*/
						for (int index = 0; index < 3; index++) {
							singleColor[index] = std::clamp(singleColor[index], 0.0f, 1.0f);
							singleColor[index] = sqrt(singleColor[index]);
						}
						Color newColor = colorImage.GetImagePoint(x, y) + singleColor;
						colorImage.SetImagePoint(x, y, newColor);
					}
				}
			}
		}
	};
	/*
		线程开始
		我的电脑只有四个线程
	*/
	constexpr int threadCount = 4;
	vector<thread> threadPool(threadCount);
	for (auto& t : threadPool) {
		t = thread(threadTask);
	}

	/*
		主线程轮询
	*/

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
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
				设置 光线数量 和 运行时间
			*/
			
			std::wstring s;
			s.reserve(50);
			s.append(L"RayTracing   RayCount : ");
			s.append(std::to_wstring(mainCaculateCount));
			s.append(L"   TotalTime : ");
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			auto t1 = std::chrono::high_resolution_clock::now();
			auto seconds = (std::chrono::duration_cast<std::chrono::seconds>(t1 - t0) % 60).count();
			auto minutes = std::chrono::duration_cast<std::chrono::minutes>(t1 - t0).count();
			s.append(std::to_wstring(minutes));
			s.append(L":");
			if (seconds < 10) {
				s.append(L"0");
			}
			s.append(std::to_wstring(seconds));
			application.SetWindowsText(s.c_str());
		}
	}

	/*
		设置退出并等待
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
		Vector unit = ray.Direction().Normalize();
		float t = 0.5f * (unit.Y() + 1.0f);
		Vector v = (1.0f - t) * Vector(1.0f, 1.0f, 1.0f) + t * Vector(0.5f, 0.7f, 1.0f);
		return Color(v.X(), v.Y(), v.Z());
		//return Color(0.0f, 0.0f, 0.0f);
	}

	/*
		修改hitRecord后
		这里会比原版更清晰
	*/
	Ray scattered;
	Color emitted;
	Color attenuation;
	record.hitable->Calculate(record.ray, record.t, scattered, emitted, attenuation);
	if (depth >= 0) {
		return emitted + attenuation * GetPixelColor(scattered, hits, depth - 1);
	} else {
		return emitted;
	}
}