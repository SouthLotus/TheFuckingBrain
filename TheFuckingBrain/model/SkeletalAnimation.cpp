#include "SkeletalAnimation.hpp"

const std::vector<std::vector<glm::mat4>>* SkeletalAnimation::getMats() const
{
	return &mats;
}

const std::string SkeletalAnimation::getName() const
{
	return name;
}

const int SkeletalAnimation::getNumBones() const
{
	return numBones;
}

const int SkeletalAnimation::getTicksPerSecond() const
{
	return ticksPerSecond;
}

const int SkeletalAnimation::getDuration() const
{
	return duration;
}

void SkeletalAnimation::setMats(std::vector<std::vector<glm::mat4>> mats)
{
	this->mats = mats;
}

void SkeletalAnimation::setNumBones(unsigned int numBones)
{
	this->numBones = numBones;
}

void SkeletalAnimation::setName(std::string name)
{
	this->name = name;
}

void SkeletalAnimation::setTicksPerSecond(unsigned int ticksPerSecond)
{
	this->ticksPerSecond = ticksPerSecond;
}

void SkeletalAnimation::setDuration(unsigned int duration)
{
	this->duration = duration;
}
