#pragma once
#include <Siv3D.hpp>
#include <thread>
#include <mutex>

#define PI 3.141592653589793f
#define SERVO_LOT 12.0f

float servo_angle_calc(float angle) {
	double servo_tilt, servo_range, internal_angle_1, internal_angle_2, table_length;
	double servo_axis_x = 0;
	double servo_axis_y = 0;
	float servo_angle;

	//angle = angle * 4 / 3;
	angle = angle * 18 / 15;

	if (angle > 20 || -20 > angle) {
		return -999.0f;
	}

	if (angle > 0) {
		servo_axis_x = 5.538f;
		servo_axis_y = 4.58f;
	}
	else {
		servo_axis_x = 4.002f;
		servo_axis_y = 4.58f;
	}

	servo_range = sqrt(pow(servo_axis_x, 2.0) + pow(servo_axis_y, 2.0));
	servo_tilt = acos(servo_axis_x / servo_range) * 180.0 / PI;
	internal_angle_1 = angle + servo_tilt;
	table_length = (servo_range * 2.0 * cos(internal_angle_1 * PI / 180.0) + sqrt(4 * pow(servo_range, 2.0) * pow(cos(internal_angle_1 * PI / 180.0), 2.0) - 4 * (pow(servo_range, 2.0) - pow(SERVO_LOT, 2.0)))) / 2;
	internal_angle_2 = (acos((pow(servo_range, 2.0) + pow(SERVO_LOT, 2.0) - pow(table_length, 2.0)) / (2.0 * servo_range * SERVO_LOT))) * 180.0 / PI;
	servo_angle = 180.0f - (float)servo_tilt - (float)internal_angle_2;

	return servo_angle;
}

template<class ReceiveCommand,class SendCommand>
class Device {
public:

	using CallBack = std::function<void(const Array<uint8>&)>;

private:
	
	Serial m_serialPort;

	uint8 m_deviceID;

	int32 m_rate;

	std::unordered_map<ReceiveCommand, CallBack> m_callBacks;

	std::unique_ptr<std::thread> m_receiveThread;

	std::unique_ptr<std::mutex> m_mtx = std::make_unique<std::mutex>();

	bool m_isReceive = false;
	
public:

	Device(uint8 deviceID, int32 rate = 9600) : m_serialPort(Serial()),m_deviceID(deviceID), m_rate(rate) {}

	~Device() {
		if (!isConnect()) {
			return;
		}

		m_serialPort.close();
	}

	bool connect() {
		if (isConnect()) {
			return true;
		}
		const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
		for (auto& itr : infos) {
			Serial serial(itr.port, m_rate);
			if (!serial.isOpen()) {
				continue;
			}
			System::Sleep(20);
			serial.clear();
			serial.clearInput();
			serial.clearOutput();
			System::Sleep(20);
			serial.writeByte(1);
			serial.writeByte(0);
			System::Sleep(20);
			auto size = serial.available();
			while (serial.available() < 4) {
				serial.writeByte(1);
				serial.writeByte(0);
				System::Sleep(20);
			}
			auto packet = serial.readBytes();
			if (m_deviceID == packet[2]) {
				m_serialPort = serial;
				m_serialPort.clear();
				return true;
			}
		}
		return false;
	}

	void disConnect() {
		m_serialPort.close();
	}

	bool isConnect() const {
		return m_serialPort.isOpen();
	}

	bool writeCommand(SendCommand command) {
		if (!isConnect()) {
			return false;
		}

		std::lock_guard<std::mutex> lock(*m_mtx);
		m_serialPort.writeByte(1);
		return 	m_serialPort.writeByte(static_cast<uint8_t>(command));
	}

	bool writeCommand(SendCommand command, void* src, uint8_t size) {
		if (!isConnect()) {
			return false;
		}

		std::lock_guard<std::mutex> lock(*m_mtx);
		m_serialPort.writeByte(size + 1);
		m_serialPort.writeByte(static_cast<uint8_t>(command));

		return m_serialPort.write(src, size) == size;
	}

	void setReceiveCallBack(ReceiveCommand command, CallBack callBack) {
		m_callBacks.emplace(command, callBack);
	}

	bool startReceive() {

		if (!isConnect()) {
			return false;
		}

		m_isReceive = true;

		m_receiveThread = std::make_unique<std::thread>([this] {receiveThread(); });

		return true;
	}

	void stopReceive() {
		m_isReceive = false;
		m_receiveThread.release();
	}

private:

	void receiveThread() {
		uint8_t packetSize = 0;

		enum class State {
			IDLE,
			RECEIVING
		};

		State state = State::IDLE;

		while (m_serialPort.isOpen() && m_isReceive) {

			switch (state)
			{
			case State::IDLE:
				if (m_serialPort.available()) {
					m_serialPort.read(packetSize);
					state = State::RECEIVING;
				}
				else {
					System::Sleep(1);
				}
				break;
			case State::RECEIVING:
				if (m_serialPort.available() >= packetSize) {
					uint8_t command;
					m_serialPort.read(command);
					Array<uint8_t> data;
					
					data.resize(packetSize - 2);

					if (packetSize - 2) {
						m_serialPort.read(&data[0], packetSize - 2);
					}

					if (auto itr = m_callBacks.find(static_cast<ReceiveCommand>(command)); itr != m_callBacks.end()) {
						itr->second(data);
					}

					uint8_t tmp;
					m_serialPort.read(tmp);
					state = State::IDLE;
				}
				break;
			default:
				break;
			}
		}

		m_isReceive = false;
	}
};