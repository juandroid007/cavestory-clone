#ifndef ACCELERATORS_H_
#define ACCELERATORS_H_

#include "units.h"
#include "kinematics.h"

struct Kinematics;

struct Accelerator {
	virtual ~Accelerator() = 0;

	virtual void updateVelocity(Kinematics& kinematics, units::MS elapsed_time) const = 0;
};

inline Accelerator::~Accelerator() {}

struct ZeroAccelerator : Accelerator {
	ZeroAccelerator() {}

	void updateVelocity(Kinematics&, units::MS) const {}

	static const ZeroAccelerator kZero;
};

struct ResetAccelerator : Accelerator {
	ResetAccelerator() {}

	void updateVelocity(Kinematics& kinematics, units::MS) const {
		kinematics.velocity = 0.0f;
	};

	static const ResetAccelerator Reset;
};

struct ConstantAccelerator : Accelerator {
	public:
		ConstantAccelerator(units::Acceleration acceleration, units::Velocity max_velocity) :
			acceleration_(acceleration),
			max_velocity_(max_velocity) {}

		void updateVelocity(Kinematics& kinematics, units::MS elapsed_time) const;

		static const ConstantAccelerator kGravity;
	private:
		units::Acceleration acceleration_;
		units::Velocity max_velocity_;
};

struct FrictionAccelerator : Accelerator {
	public:
		FrictionAccelerator(units::Acceleration friction) :
			friction_(friction) {}

		void updateVelocity(Kinematics& kinematics, units::MS elapsed_time) const;
	private:
		const units::Acceleration friction_;
};

struct BidirectionalAccelerators {
	BidirectionalAccelerators(units::Acceleration acceleration, units::Velocity max_velocity) :
		positive(acceleration, max_velocity),
		negative(-acceleration, -max_velocity) {}

	ConstantAccelerator positive, negative;
};

extern const units::Velocity kTerminalSpeed;

#endif // ACCELERATORS_H_
