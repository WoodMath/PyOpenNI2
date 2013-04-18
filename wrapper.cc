#include <boost/python/detail/wrap_python.hpp>
#include <OpenNI.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
#include <string>
#include <sstream>

#include "utils.h"

using namespace boost::python;
using boost::function;
using boost::noncopyable;
using boost::apply;
using boost::bind;
using boost::mpl::vector;

using openni::OpenNI;



/////////////////////
// Primitive Types //
/////////////////////

namespace Version {
  std::string __repr__(const openni::Version& v) {
    std::stringstream str;
    str << "openni2.Version(\"" << v.major
                         << "." << v.minor
                         << "." << v.maintenance
                         << "." << v.build
        << "\")";
    return str.str();
  }
  std::string __str__(const openni::Version& v) {
    std::stringstream str;
    str      << v.major
      << "." << v.minor
      << "." << v.maintenance
      << "." << v.build
    ;
    return str.str();
  }
}

inline void primitives() {
  // ENUM: DeviceState
  enum_<openni::DeviceState>("DeviceState")
    .value("OK", openni::DEVICE_STATE_OK)
    .value("ERROR", openni::DEVICE_STATE_ERROR)
    .value("NOT_READY", openni::DEVICE_STATE_NOT_READY)
    .value("EOF", openni::DEVICE_STATE_EOF)
  ;
  
  // ENUM: ImageRegistrationMode
  enum_<openni::ImageRegistrationMode>("ImageRegistrationMode")
    .value("OFF", openni::IMAGE_REGISTRATION_OFF)
    .value("DEPTH_TO_COLOR", openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR)
  ;

  // ENUM: PixelFormat
  enum_<openni::PixelFormat>("PixelFormat")
    .value("DEPTH_1_MM", openni::PIXEL_FORMAT_DEPTH_1_MM)
    .value("DEPTH_100_UM", openni::PIXEL_FORMAT_DEPTH_100_UM)
    .value("SHIFT_9_2", openni::PIXEL_FORMAT_SHIFT_9_2)
    .value("SHIFT_9_3", openni::PIXEL_FORMAT_SHIFT_9_3)
    .value("RGB888", openni::PIXEL_FORMAT_RGB888)
    .value("YUV422", openni::PIXEL_FORMAT_YUV422)
    .value("GRAY8", openni::PIXEL_FORMAT_GRAY8)
    .value("GRAY16", openni::PIXEL_FORMAT_GRAY16)
    .value("JPEG", openni::PIXEL_FORMAT_JPEG)
  ;

  // ENUM: SensorType
  enum_<openni::SensorType>("SensorType")
    .value("IR", openni::SENSOR_IR)
    .value("COLOR", openni::SENSOR_COLOR)
    .value("DEPTH", openni::SENSOR_DEPTH)
  ;
  
  // ENUM: Status
  enum_<openni::Status>("Status")
    .value("OK", openni::STATUS_OK)
    .value("ERROR", openni::STATUS_ERROR)
    .value("NOT_IMPLEMENTED", openni::STATUS_NOT_IMPLEMENTED)
    .value("NOT_SUPPORTED", openni::STATUS_NOT_SUPPORTED)
    .value("BAD_PARAMETER", openni::STATUS_BAD_PARAMETER)
    .value("OUT_OF_FLOW", openni::STATUS_OUT_OF_FLOW)
    .value("NO_DEVICE", openni::STATUS_NO_DEVICE)
    .value("TIME_OUT", openni::STATUS_TIME_OUT)
  ;
  
  scope().attr("ANY_DEVICE") = openni::ANY_DEVICE;
  scope().attr("TIMEOUT_FOREVER") = openni::TIMEOUT_FOREVER;
  scope().attr("TIMEOUT_NONE") = openni::TIMEOUT_NONE;
  
  // The Version struct //FIXME: reduce
  class_<openni::Version>("Version")
    .def_readwrite("major", &openni::Version::major)
    .def_readwrite("minor", &openni::Version::minor)
    .def_readwrite("maintenance", &openni::Version::maintenance)
    .def_readwrite("build", &openni::Version::build)
    .def("__repr__", &Version::__repr__)
    .def("__str__", &Version::__str__)
  ;
  scope().attr("version") = OpenNI::getVersion();
  
  //TODO: array, listeners converter
}



////////////////////////
// Exception wrapping //
////////////////////////

// Define the exception class
class StatusError : public std::exception {
public:
  StatusError(openni::Status status): status_(status), msg_(NULL) {}
  StatusError(openni::Status status, const char* msg): status_(status), msg_(msg) {}
  openni::Status const status_;
  const char*    const msg_;
  std::string repr() {
    std::string status = extract<std::string>(str(status_));
    std::stringstream ret;
    ret << "openni2.Error(status=" << status << "(" << status_ << ")";
    if (msg_) ret << ", msg=\"...\"";
    ret << ")";
    return ret.str();
  }
  //TODO: __str__ and __repr__
};
std::ostream& operator<<(std::ostream& o, const StatusError& er) {
  std::string status = extract<std::string>(str(er.status_));
  o << status << " (status " << er.status_ << ")";
  if (er.msg_) o << ":\n" << er.msg_;
  return o;
}

// Wrap it
object errtype;
inline void error() {
  errtype = class_<StatusError>("Error", init<openni::Status, const char*>())
    .def(init<openni::Status>())
    .def_readonly("status", &StatusError::status_)
    .def_readonly("msg", &StatusError::msg_)
    .def(self_ns::str(self_ns::self))
    .def("__repr__", &StatusError::repr)
  ;
  //FIXME: make it inherit from exception
}

// Define check() and a wrapper for it
void check(openni::Status ret) {
  if (ret == openni::STATUS_OK) return;
  object err = errtype(ret, OpenNI::getExtendedError());
  PyErr_SetObject(errtype.ptr(), err.ptr());
  throw error_already_set();
}
//////// Noooooo! A lowercased-named macro in a C++0x file! Shoot him!
//////#define checking(func) bind(check, bind(func, _1))




///////////////////////
// Version utilities //
///////////////////////

inline void version() {
  //TODO
}



////////////////
// Main types //
////////////////

namespace VideoMode {
  tuple getResolution(const openni::VideoMode& vm) {
    return make_tuple(vm.getResolutionX(), vm.getResolutionY());
  }
  void setResolution(openni::VideoMode& vm, tuple& res) {
    vm.setResolution(extract<int>(res[0]), extract<int>(res[1]));
  }
}
namespace SensorInfo {
  tuple getSupportedVideoModes(const openni::SensorInfo& info) {
    return convertArray(info.getSupportedVideoModes()); //DANGLING
  }
}
namespace PlaybackControl {
  void setSpeed(openni::PlaybackControl& pb, float value) {
    check(pb.setSpeed(value));
  }
  void seek(openni::PlaybackControl& pb, const openni::VideoStream& s, int frame) {
    check(pb.seek(s, frame));
  }
}

inline void mainTypes() {
  // INFORMATION CARRIER TYPES
  class_<openni::CameraSettings>("CameraSettings", no_init)
    .add_property("auto_exposure", &openni::CameraSettings::getAutoExposureEnabled,
                                   &openni::CameraSettings::setAutoExposureEnabled)
    .add_property("auto_white_balance", &openni::CameraSettings::getAutoWhiteBalanceEnabled,
                                        &openni::CameraSettings::setAutoWhiteBalanceEnabled)
    .add_property("exposure", &openni::CameraSettings::getExposure,
                              &openni::CameraSettings::setExposure)
    .add_property("gain", &openni::CameraSettings::getGain,
                          &openni::CameraSettings::setGain)
  
    .add_property("valid", &openni::CameraSettings::isValid)
  ;
  class_<openni::DeviceInfo>("DeviceInfo", no_init)
    .add_property("name", &openni::DeviceInfo::getName)
    .add_property("uri", &openni::DeviceInfo::getUri)
    .add_property("vendor", &openni::DeviceInfo::getVendor)
    .add_property("usb_product_id", &openni::DeviceInfo::getUsbProductId)
    .add_property("usb_vendor_id", &openni::DeviceInfo::getUsbVendorId)
  ;
  class_<openni::VideoMode>("VideoMode")
    .add_property("fps", &openni::VideoMode::getFps,
                         &openni::VideoMode::setFps)
    .add_property("pixel_format", &openni::VideoMode::getPixelFormat,
                                  &openni::VideoMode::setPixelFormat)
    .add_property("resolution", &VideoMode::getResolution,
                                &VideoMode::setResolution)
  ;
  class_<openni::SensorInfo, noncopyable>("SensorInfo", no_init)
    .add_property("type", &openni::SensorInfo::getSensorType)
    .add_property("video_modes", &SensorInfo::getSupportedVideoModes)
  ;


  // THE INTERESTING THINGS

  // Define 'em first
  class_<openni::VideoStream, noncopyable> videoStream ("VideoStream");
  class_<openni::PlaybackControl> playbackControl("PlaybackControl", no_init);
  class_<openni::Device, noncopyable> device ("Device");

  device

    //flags
    .add_property("file", &openni::Device::isFile)
    .add_property("valid", &openni::Device::isValid)

    //properties
    .add_property("sync_enabled", &openni::Device::getDepthColorSyncEnabled,
                                  &openni::Device::setDepthColorSyncEnabled)
    .add_property("img_reg_mode", &openni::Device::getImageRegistrationMode,
                                  &openni::Device::setImageRegistrationMode)

//TODO
//    //control objects
//    .add_property("info", &openni::Device::getDeviceInfo) //DANGLING
//    .add_property("playback_control", &openni::Device::getPlaybackControl) //DANGLING
//
//    //methods
//    .def("open", &openni::Device::open, return_self<>())
//    .def("has_sensor", &openni::Device::hasSensor)
//    .def("get_sensor_info", &openni::Device::getSensorInfo) //DANGLING
//    .def("is_img_reg_mode_supported", &openni::Device::isImageRegistrationModeSupported)

  ;
  
  videoStream

    //TODO

  ;


  // CONTROL TYPES (STILL INTERESTING)

  playbackControl

    //flags
    .add_property("valid", &openni::PlaybackControl::isValid)

    //properties
    .add_property("repeating", &openni::PlaybackControl::getRepeatEnabled,
                               &openni::PlaybackControl::setRepeatEnabled)
    .add_property("speed", &openni::PlaybackControl::getSpeed,
                           &PlaybackControl::setSpeed)

    //methods
    .def("get_number_of_frames", &openni::PlaybackControl::getNumberOfFrames)
    .def("seek", &PlaybackControl::seek)

  ;

}



//////////////////
// "Static" API //
//////////////////

// Callbacks //TODO: solve segfault bug on exit
CALLER( devConn
      , OpenNI, DeviceConnected
      , (const openni::DeviceInfo* info)
      , bind(apply<object>(), _1, info) )
CALLER( devDisconn
      , OpenNI, DeviceDisconnected
      , (const openni::DeviceInfo* info)
      , bind(apply<object>(), _1, info) )
CALLER( devState
      , OpenNI, DeviceStateChanged
      , (const openni::DeviceInfo* info, openni::DeviceState state)
      , bind(apply<object>(), _1, info, state) )

CALLERS( {"deviceConnected", &devConn}
       , {"deviceDisconnected", &devDisconn}
       , {"deviceStateChanged", &devState}
       )

// Initialize / Shutdown logic
void initialize() {
  check(OpenNI::initialize());
  // Register callback handlers //FIXME: is this right? callback blocking?
  REG_CALLER(devConn, OpenNI, DeviceConnected);
  REG_CALLER(devDisconn, OpenNI, DeviceDisconnected);
  REG_CALLER(devState, OpenNI, DeviceStateChanged);
}
void shutdown() {
  OpenNI::shutdown();
}

// Other top-level functions
tuple enumerateDevices() {
  openni::Array<openni::DeviceInfo> arr;
  OpenNI::enumerateDevices(&arr);
  return convertArray(arr); //DANGLING
}

// Expose everything
inline void mainFunc() {
  def("init", initialize);
  def("shutdown", shutdown);
  def("on", __on);
  def("off", __off0);
  def("off", __off1);
  def("off", __off2);
  def("enumerate_devices", enumerateDevices);
}



///////////////////////////////////////////////////////////////////////////////

BOOST_PYTHON_MODULE(openni2) {
  // Customize what will appear on the docstrings:
  docstring_options local_docstring_options(
    true, // custom docstring
    true, // Python signature
    false // C++ signature
  );
  
  primitives();
  error();
  version();
  mainTypes();
  mainFunc();
}

