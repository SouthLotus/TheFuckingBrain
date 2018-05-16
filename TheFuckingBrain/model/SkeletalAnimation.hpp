#ifndef __skeletal_animation
#define __skeletal_animation
#include <string>
#include <vector>
#include <glm/glm.hpp>

/*
Luu tru cac thong tin can thiet cho animation
*/
class SkeletalAnimation {
private:
	std::string name;
	//ticks - bone
	std::vector<std::vector<glm::mat4>> mats;
	unsigned int numBones;
	unsigned int ticksPerSecond;
	unsigned int duration;
public:
	SkeletalAnimation() = default;
	~SkeletalAnimation() = default;
	const std::vector<std::vector<glm::mat4>> *getMats() const;
	const std::string getName() const;
	const int getNumBones() const;
	const int getTicksPerSecond() const;
	const int getDuration() const;
	void setMats(
		std::vector<std::vector<glm::mat4>> mats);
	void setNumBones(unsigned int numBones);
	void setName(std::string name);
	void setTicksPerSecond(unsigned int ticksPerSecond);
	void setDuration(unsigned int duration);
};
#endif