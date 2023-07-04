#include "hardware.h"
void Core::setType(int type) {
	switch (type) {
	case CPU_TYPE_E:
		this->type = CPU_TYPE_E;
		work_rate = 1;
		regular_consumption = 1;
		starting_consumption = 0.1;
		break;
	case CPU_TYPE_P:
		this->type = CPU_TYPE_P;
		work_rate = 2;
		regular_consumption = 3;
		starting_consumption = 0.5;
		break;
	case CPU_TYPE_NONE:
		this->type = CPU_TYPE_NONE;
		regular_consumption = 0;
		starting_consumption = 0;
		break;
	}
	return;
}

void Core::activate() {
	total_consumption += starting_consumption;
}

void Processor::get_power_usage(std::vector<double>& power_usage){
	for (int i = 0; i < P; i++)
		power_usage[i] = (double)core[i].total_consumption;
	return;
}
