#pragma once
#include <Siv3D.hpp>
#include "Device.hpp"
#include <mutex>

class Seesaw {
public:

	enum class SendCommand {
		WHO_AN_I,
		WAKE_UP,
		SLEEP,
		GET_ANGLE,
		BUTTON_LED
	};

	enum class ReceiveCommand {
		ANGLE,
		BUTTON_PUSH
	};

private:

	std::shared_ptr<Device<ReceiveCommand, SendCommand>> m_device;

	float m_angle = 0.0f;

	bool m_isPushed = false;

	std::mutex m_mtx;

public:

	Seesaw(std::shared_ptr<Device<ReceiveCommand, SendCommand>> device) :m_device(device) {
		m_device->writeCommand(SendCommand::WAKE_UP);

		m_device->setReceiveCallBack(ReceiveCommand::ANGLE, [this](const Array<uint8>& data) {
			std::lock_guard lock(m_mtx);
			if (data.size() >= 4) {
				memcpy(&m_angle, &data[0], sizeof(float));
			}
			m_device->writeCommand(SendCommand::GET_ANGLE);
		});

		m_device->setReceiveCallBack(ReceiveCommand::BUTTON_PUSH, [this](const Array<uint8>& data) {
			m_isPushed = true;
		});

		m_device->writeCommand(SendCommand::GET_ANGLE);
	}

	~Seesaw() {
		m_device->stopReceive();
		m_device->writeCommand(SendCommand::SLEEP);
	}

	void init() {
		m_isPushed = false;
	}

	float getAngle() {
		std::lock_guard lock(m_mtx);
		return m_angle;
	}

	const std::shared_ptr<Device<ReceiveCommand, SendCommand>>& getDevice() {
		return m_device;
	}

	bool isPushed() {
		bool result = m_isPushed;

		m_isPushed = false;

		return result;
	}

	void buttonLED(uint8_t type) {
		m_device->writeCommand(SendCommand::BUTTON_LED,reinterpret_cast<void*>(const_cast<uint8_t*>(&type)), sizeof(type));

	}

};