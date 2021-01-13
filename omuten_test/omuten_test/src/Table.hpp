#pragma once
#include "Device.hpp"
#define INIT_POS_X 15.0f
#define INIT_POS_Y 15.0f

//Float4 SeesawToServoAngle(float x,float y) {
Float4 SeesawToServoAngle(float y,float x) {
	Float4 float4;
	x *= (-1);
	float4.x = servo_angle_calc(static_cast<float>(x)) + 90.0f;
	float4.y = servo_angle_calc(static_cast<float>(x * -1.0f)) + 90.0f;
	float4.z = servo_angle_calc(static_cast<float>(y)) + 90.0f;
	float4.w = servo_angle_calc(static_cast<float>(y * -1.0f)) + 90.0f;
	if(float4.x<0.0f){
		float4.x+=5.0f;
	}
	float4.y+=3.2f;
	float4.z+=5.0f;
	float4.w+=5.0f;
	
	return float4;
}

class Table {
public:

	enum class SendCommand {
		WHO_AN_I,
		WAKE_UP,
		SLEEP,
		TABLE_ROTATE,
		BOAL_EMISSION,
		BOAL_EMISSION_INIT,
		TIME_LED,
		DECORATION_LED,
		BONUS_LED,

	};

	enum class ReceiveCommand {
		GOAL,
		BONUS
	};

private:

	std::shared_ptr<Device<ReceiveCommand, SendCommand>> m_device;

	bool m_isGoal = false;

	bool m_isBonus = false;

	bool m_bnsBlinkFlg = false;

public:

	Table(std::shared_ptr<Device<ReceiveCommand, SendCommand>> device) :m_device(device) {
		m_device->setReceiveCallBack(ReceiveCommand::GOAL,  [this](const Array<uint8>&) {
			m_isGoal  = true;
		});

		m_device->setReceiveCallBack(ReceiveCommand::BONUS, [this](const Array<uint8>&) {
			m_isBonus = true;
		});

		m_device->writeCommand(SendCommand::WAKE_UP);
	}

	~Table() {
		m_device->writeCommand(SendCommand::SLEEP);
	}

	void init() {
		m_isGoal  = false;
		m_isBonus = false;
	}

	bool setServoAngle(const Float4& float4) {
		return m_device->writeCommand(SendCommand::TABLE_ROTATE, reinterpret_cast<void*>(const_cast<Float4*>(&float4)), sizeof(float) * 4);
	}

	bool boalEmission() {
		return m_device->writeCommand(SendCommand::BOAL_EMISSION);
	}
	bool boalEmission_init() {
		return m_device->writeCommand(SendCommand::BOAL_EMISSION_INIT);
	}

	bool turnOnTimeLED(const Color& color) {
		return m_device->writeCommand(SendCommand::TIME_LED, reinterpret_cast<void*>(const_cast<Color*>(&color)), sizeof(Color));
	}

	bool turnOnDecorationLED(const Color& xColor, const Color& yColor,const Float2& float2) {
		Array<uint8_t> packet;
//		Print << U"x:" << float2;
		packet.resize(sizeof(Color) * 2 + sizeof(Float2));

		std::memcpy(&packet[0], reinterpret_cast<void*>(const_cast<Color*>(&xColor)), sizeof(Color));
		std::memcpy(&packet[sizeof(Color)], reinterpret_cast<void*>(const_cast<Color*>(&yColor)), sizeof(Color));
		std::memcpy(&packet[sizeof(Color)*2], reinterpret_cast<void*>(const_cast<Float2*>(&float2)), sizeof(Float2));//y‚ª
		
	//	Print <<U"packet:"<< packet;
		return m_device->writeCommand(SendCommand::DECORATION_LED, &packet[0], packet.size());
	}

	bool bonusLED(const uint8_t& type) {
		return m_device->writeCommand(SendCommand::BONUS_LED, reinterpret_cast<void*>(const_cast<uint8_t*>(&type)), sizeof(uint8_t));
	}


	bool isGoal() {
		bool result = m_isGoal;

		m_isGoal = false;

		return result;
	}

	bool isBonus() {
		bool result = m_isBonus;

		m_isBonus = false;

		return result;
	}

	const std::shared_ptr<Device<ReceiveCommand, SendCommand>>& getDevice() {
		return m_device;
	}
};