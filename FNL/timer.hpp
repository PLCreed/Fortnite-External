#pragma once




class c_timer {
private:
	std::chrono::high_resolution_clock::time_point strt;
public:

	void start() {
		strt = std::chrono::high_resolution_clock::now();
	}

	int end() {
		auto end = std::chrono::high_resolution_clock::now();   // end of clock
		std::chrono::duration<double> time_s = end - strt;
		std::chrono::milliseconds time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_s);
		return static_cast<int>(time_ms.count());
	}

	void sleep_ms(unsigned int delay) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}

	void q_sleep(int ms) {
		LONGLONG timerResolution{}, wantedTime{}, currentTime{};

		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&timerResolution));
		timerResolution /= 1000;

		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
		wantedTime = (currentTime + ms * timerResolution) / timerResolution;
		currentTime = 0;

		while (wantedTime > 0) {
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
			currentTime /= timerResolution;
			wantedTime -= currentTime;
		}
	}

	void acc_sleep(unsigned int delay) {
		auto s_clock = std::chrono::high_resolution_clock::now();
		auto e_clock = std::chrono::high_resolution_clock::now();   // end of clock
		std::chrono::duration<double> time_s = e_clock - s_clock;
		std::chrono::milliseconds time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_s);
		while (time_ms.count() < delay) {
			e_clock = std::chrono::high_resolution_clock::now();   // end of clock
			time_s = e_clock - s_clock;
			time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_s);
		}
	}

	bool held_button(unsigned int vk_code, unsigned int delay) {
		auto s_clock = std::chrono::high_resolution_clock::now();
		while (GetAsyncKeyState(vk_code)) {
			auto e_clock = std::chrono::high_resolution_clock::now();   // end of clock
			std::chrono::duration<double> time_s = e_clock - s_clock;
			std::chrono::milliseconds time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_s);
			if (time_ms.count() >= delay) return true;
		}
		return false;
	}

	void until_press(unsigned int vk_code, unsigned int vk_code2) {
		while (true) {
			if ((GetAsyncKeyState(vk_code) & 0x8000) && (GetAsyncKeyState(vk_code2) & 0x8000)) break;
			else sleep_ms(5);
		}
	}
};