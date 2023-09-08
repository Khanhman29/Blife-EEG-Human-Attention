#include <iostream>
#include <lsl_cpp.h>
#include <vector>
#include <chrono>
#include <string>
#include <thread>

/**
 * This is a minimal example that demonstrates how a multi-channel stream (here 128ch) of a
 * particular name (here: SimpleStream) can be resolved into an inlet, and how the raw sample data &
 * time stamps are pulled from the inlet. This example does not display the obtained data.
 */




class Inlet {
private:
	const int nchannels = 8;
	std::string name = "HMIResultData";
	//int samplingrate = 2;
	int max_buffered = 360;
	std::string type = "Markers";
	int max_samples = 8;
	lsl::stream_inlet* inlet;
	std::vector<float> sample;
	//std::vector<std::vector<float>> chunk_nested_vector;
	std::chrono::high_resolution_clock::time_point next_sample_time;
	//int32_t sample_dur_us = 1000000 / (samplingrate > 0 ? samplingrate : 100); //microsecond
	bool readyReceive = false;
public:
	Inlet() {
	}

	void init() {
		// resolve the stream of interet
		std::cout << "Now resolving streams..." << std::endl;
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", name);
		if (results.empty()) throw std::runtime_error("No stream found");

		std::cout << "Here is what was resolved: " << std::endl;
		std::cout << results[0].as_xml() << std::endl;

		// make an inlet to get data from it
		std::cout << "Now creating the inlet..." << std::endl;
		inlet = new lsl::stream_inlet(results[0]);
		std::cout << "Now pulling samples..." << std::endl;
		this->readyReceive = true;
		next_sample_time = std::chrono::high_resolution_clock::now();// nanosecond
	}

	void startReceive() {
		if (this->readyReceive) {
			try {
				auto now = std::chrono::high_resolution_clock::now();
				inlet->pull_sample(sample);
			}
			catch (std::exception& e) {
				std::cerr << "Got an exception: " << e.what() << std::endl;
			}
		}
	}
	float getLastestData() {
		if (!sample.empty()) {
			float data = sample.back();
			//sample.pop_back();
			// check data to boost 1 time
			//if not pop_back, main loop time > time pull data. So we push more time than 1 
			//if (data ==1) sample.pop_back();	
			return data;
		}
		else {
			return 0;
		}
	}
};


class Outlet {
private:
	const int nchannels = 2;
	std::string name = "Unity.ExampleStream";
	int samplingrate = 50;// send n sample per second
	int max_buffered = 360;
	std::string type = "Markers";
	const char* channels[2] = { "posX", "posY" };
	lsl::stream_outlet* outlet;
	std::chrono::high_resolution_clock::time_point next_sample_time;
	int32_t sample_dur_us = 1000000 / (samplingrate > 0 ? samplingrate : 100); //microsecond
	std::vector<float> sample;

public:
	Outlet() {
		try {
			// make a new stream_info (100 Hz)
			lsl::stream_info info(
				name, type, nchannels, samplingrate, lsl::cf_float32, std::string(name));
			// add some description fields
			info.desc().append_child_value("manufacturer", "LSL");
			lsl::xml_element chns = info.desc().append_child("channels");
			for (int k = 0; k < nchannels; k++)
				chns.append_child("channel")
				.append_child_value("label", k < 8 ? channels[k] : "Chan-" + std::to_string(k + 1))
				.append_child_value("type", type);

			// make a new outlet
			outlet = new lsl::stream_outlet(info, 0, max_buffered);
			sample = std::vector<float>(nchannels, 0.0);

			next_sample_time = std::chrono::high_resolution_clock::now();// nanosecond

		}
		catch (std::exception& e) {
			std::cerr << "Got an exception: " << e.what() << std::endl;
		}
	}

	int32_t getSampleDuration() {
		return sample_dur_us;
	}

	void startSend(int NumOfPos) {
		try {
			auto now = std::chrono::high_resolution_clock::now();
			
			if (now >= next_sample_time) {
				sample[0] = NumOfPos;
				// send the sample
				std::cout << sample[0] << std::endl;
				outlet->push_sample(sample);
				//outlet->push_chunk(sample);
				//outlet->push_chunk_multiplexed(sample);
				next_sample_time += std::chrono::microseconds(sample_dur_us);
			}
			
		}
		catch (std::exception& e) {
			std::cerr << "Got an exception: " << e.what() << std::endl;
		}
	}
};

