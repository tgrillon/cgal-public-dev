#ifndef CGAL_ANIMATION_MANAGER_H
#define CGAL_ANIMATION_MANAGER_H

#include <vector>
#include <chrono>

#include "utils.h"

using namespace std::chrono_literals;

struct CameraKeyFrame {
    vec3f position;
    quatf orientation;

    CameraKeyFrame(const vec3f& _position, const quatf& _orientation)
    : position(_position), 
      orientation(_orientation) 
    {
    }

    CameraKeyFrame() : position(vec3f::Identity()), orientation(quatf::Identity()) {}
};

class Animation_controller 
{
public:
  using DurationType = std::chrono::milliseconds;
  using TimePoint = std::chrono::_V2::steady_clock::time_point;
  using KeyFrameBuffer = std::vector<CameraKeyFrame>;
public:
  Animation_controller() 
  : m_duration(2s), 
    m_startTime(), 
    m_isRunning(false),
    m_lastFrame(0.00), 
    m_currentFrame(0.00), 
    m_timestamp(0.00),
    m_lastFrameData(),
    m_interpolatedRotation(quatf::Identity()),
    m_interpolatedTranslation(vec3f::Identity())
  {
  }

  inline 
  bool is_running() const { return m_isRunning; }

  inline 
  void start() 
  {
    std::cout << "animation start" << '\n';
    if (!m_isRunning) 
    {
      m_startTime = std::chrono::steady_clock::now();
      m_isRunning = true;
    }
  }

  inline 
  CameraKeyFrame run() 
  {
    if (m_keyFrames.size() <= 1) 
    {
      stop(0.00);
      return m_lastFrameData;
    }

    auto now = std::chrono::steady_clock::now();
    double elapsedTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime).count());
    m_currentFrame = m_lastFrame + elapsedTime;
    if (m_currentFrame >= static_cast<double>(m_duration.count())) 
    {
      m_currentFrame = 0.00;
      stop(0.00);
      return m_lastFrameData;
    }

    m_lastFrameData = key_frame_interpolation(m_currentFrame); 
    return m_lastFrameData;
  }

  inline 
  void stop(const double frame) 
  {
    std::cout << "animation stopped" << '\n';
    m_isRunning = false;
    m_lastFrame = frame;
  }

  inline 
  void add_key_frame(const vec3f& position, const quatf& orientation) 
  { 
    CameraKeyFrame keyFrame(position, orientation);
    std::cout << "key frame added" << '\n';
    m_keyFrames.push_back(keyFrame);
    if (m_keyFrames.size() > 1) 
    {
      compute_timestamp();
    } 
  }

  inline 
  void set_duration(DurationType d) 
  { 
    m_duration = d; 
    if (m_keyFrames.size() > 1) 
    {
      compute_timestamp();
    }
  }

  inline 
  void clear_buffer() 
  { 
    m_keyFrames.clear(); 
  }

  inline 
  quatf get_rotation() const  
  {
    return m_interpolatedRotation;
  }

  inline 
  vec3f get_translation() const  
  {
    return m_interpolatedTranslation;
  }

  inline 
  double get_frame() const 
  {
    return m_currentFrame;
  }
  
private:
  const double EPSILON = 0.001;

  inline 
  CameraKeyFrame key_frame_interpolation(const double time)
  {
    assert(m_timestamp > 0.00 + EPSILON || m_timestamp < 0.00 - EPSILON);

    double t = time / m_timestamp;

    unsigned int lowerIndex = std::floor(t);
    unsigned int upperIndex = std::ceil(t);

    CameraKeyFrame keyFrame0 = m_keyFrames.at(lowerIndex); 
    CameraKeyFrame keyFrame1 = m_keyFrames.at(upperIndex);

    t -= static_cast<double>(lowerIndex); 

    m_interpolatedRotation = keyFrame0.orientation.slerp(t, keyFrame1.orientation); 

    m_interpolatedTranslation = lerp(keyFrame0.position, keyFrame1.position, t);

    return {m_interpolatedTranslation, m_interpolatedRotation};
  }

  inline 
  void compute_timestamp() 
  { 
    assert(m_keyFrames.size() > 1);
    m_timestamp = static_cast<double>(m_duration.count()) / static_cast<double>(m_keyFrames.size() - 1); 
  }

private: 
  double m_lastFrame;  
  double m_currentFrame;  
  double m_timestamp;  

  quatf m_interpolatedRotation;
  vec3f m_interpolatedTranslation;

  KeyFrameBuffer m_keyFrames;

  DurationType m_duration;   

  TimePoint m_startTime; 

  CameraKeyFrame m_lastFrameData;

  bool m_isRunning;
};

#endif // CGAL_ANIMATION_MANAGER_H
