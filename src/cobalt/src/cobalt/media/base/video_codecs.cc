// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cobalt/media/base/video_codecs.h"

#include <vector>

#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "starboard/memory.h"
#include "starboard/string.h"

namespace cobalt {
namespace media {

// The names come from src/third_party/ffmpeg/libavcodec/codec_desc.c
std::string GetCodecName(VideoCodec codec) {
  switch (codec) {
    case kUnknownVideoCodec:
      return "unknown";
    case kCodecH264:
      return "h264";
    case kCodecHEVC:
      return "hevc";
    case kCodecDolbyVision:
      return "dolbyvision";
    case kCodecVC1:
      return "vc1";
    case kCodecMPEG2:
      return "mpeg2video";
    case kCodecMPEG4:
      return "mpeg4";
    case kCodecTheora:
      return "theora";
    case kCodecVP8:
      return "vp8";
    case kCodecVP9:
      return "vp9";
    case kCodecAV1:
      return "av1";
  }
  NOTREACHED();
  return "";
}

std::string GetProfileName(VideoCodecProfile profile) {
  switch (profile) {
    case VIDEO_CODEC_PROFILE_UNKNOWN:
      return "unknown";
    case H264PROFILE_BASELINE:
      return "h264 baseline";
    case H264PROFILE_MAIN:
      return "h264 main";
    case H264PROFILE_EXTENDED:
      return "h264 extended";
    case H264PROFILE_HIGH:
      return "h264 high";
    case H264PROFILE_HIGH10PROFILE:
      return "h264 high 10";
    case H264PROFILE_HIGH422PROFILE:
      return "h264 high 4:2:2";
    case H264PROFILE_HIGH444PREDICTIVEPROFILE:
      return "h264 high 4:4:4 predictive";
    case H264PROFILE_SCALABLEBASELINE:
      return "h264 scalable baseline";
    case H264PROFILE_SCALABLEHIGH:
      return "h264 scalable high";
    case H264PROFILE_STEREOHIGH:
      return "h264 stereo high";
    case H264PROFILE_MULTIVIEWHIGH:
      return "h264 multiview high";
    case HEVCPROFILE_MAIN:
      return "hevc main";
    case HEVCPROFILE_MAIN10:
      return "hevc main 10";
    case HEVCPROFILE_MAIN_STILL_PICTURE:
      return "hevc main still-picture";
    case VP8PROFILE_ANY:
      return "vp8";
    case VP9PROFILE_PROFILE0:
      return "vp9 profile0";
    case VP9PROFILE_PROFILE1:
      return "vp9 profile1";
    case VP9PROFILE_PROFILE2:
      return "vp9 profile2";
    case VP9PROFILE_PROFILE3:
      return "vp9 profile3";
    case DOLBYVISION_PROFILE0:
      return "dolby vision profile 0";
    case DOLBYVISION_PROFILE4:
      return "dolby vision profile 4";
    case DOLBYVISION_PROFILE5:
      return "dolby vision profile 5";
    case DOLBYVISION_PROFILE7:
      return "dolby vision profile 7";
    case THEORAPROFILE_ANY:
      return "theora";
    case AV1PROFILE_PROFILE_MAIN:
      return "av1 profile main";
    case AV1PROFILE_PROFILE_HIGH:
      return "av1 profile high";
    case AV1PROFILE_PROFILE_PRO:
      return "av1 profile pro";
  }
  NOTREACHED();
  return "";
}

bool ParseAv1CodecId(const std::string& codec_id, VideoCodecProfile* profile,
                     uint8_t* level_idc, gfx::CbColorSpace* color_space) {
  // The codecs parameter string for the AOM AV1 codec is as follows:
  // See https://aomediacodec.github.io/av1-isobmff/#codecsparam.
  //
  // <sample entry4CC>.<profile>.<level><tier>.<bitDepth>.<monochrome>.
  // <chromaSubsampling>.<colorPrimaries>.<transferCharacteristics>.
  // <matrixCoefficients>.<videoFullRangeFlag>

  // TODO: Replace the following code using
  //         std::vector<std::string> fields = base::SplitString(
  //             codec_id, ".", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);
  //       once Chromium base rebase is finished.
  if (SbStringFindCharacter(codec_id.c_str(), ' ') != nullptr) {
    return false;
  }
  if (SbStringFindString(codec_id.c_str(), "..") != nullptr) {
    return false;
  }
  std::vector<std::string> fields = base::SplitString(
      codec_id, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  // The parameters sample entry 4CC, profile, level, tier, and bitDepth are all
  // mandatory fields. If any of these fields are empty, or not within their
  // allowed range, the processing device SHOULD treat it as an error.
  if (fields.size() < 4 || fields.size() > 10) {
    DVLOG(3) << __func__ << " Invalid number of fields (" << fields.size()
             << ")";
    return false;
  }

  // All the other fields (including their leading '.') are optional, mutually
  // inclusive (all or none) fields. If not specified then the values listed in
  // the table below are assumed.
  //
  // mono_chrome              0
  // chromaSubsampling        112 (4:2:0 colocated with luma (0,0))
  // colorPrimaries           1 (ITU-R BT.709)
  // transferCharacteristics  1 (ITU-R BT.709)
  // matrixCoefficients       1 (ITU-R BT.709)
  // videoFullRangeFlag       0 (studio swing representation)
  *color_space = gfx::CbColorSpace::CreateREC709();

  if (fields[0] != "av01") {
    DVLOG(3) << __func__ << " Invalid AV1 4CC (" << fields[0] << ")";
    return false;
  }

  // The level parameter value SHALL equal the first level value indicated by
  // seq_level_idx in the Sequence Header. The tier parameter value SHALL be
  // equal to M when the first seq_tier value in the Sequence Header is equal to
  // 0, and H when it is equal to 1.
  if (fields[2].size() != 3 || (fields[2][2] != 'M' && fields[2][2] != 'H')) {
    DVLOG(3) << __func__ << " Invalid level+tier (" << fields[2] << ")";
    return false;
  }

  // Since tier has been validated, strip the trailing tier indicator to allow
  // int conversion below.
  fields[2].resize(2);

  // Fill with dummy values to ensure parallel indices with fields.
  std::vector<int> values(fields.size(), 0);
  for (size_t i = 1; i < fields.size(); ++i) {
    if (fields[i].empty()) {
      DVLOG(3) << __func__ << " Invalid empty field (position:" << i << ")";
      return false;
    }

    if (!base::StringToInt(fields[i], &values[i]) || values[i] < 0) {
      DVLOG(3) << __func__ << " Invalid field value (" << values[i] << ")";
      return false;
    }
  }

  // The profile parameter value, represented by a single digit decimal, SHALL
  // equal the value of seq_profile in the Sequence Header.
  const int profile_idc = fields[1].size() == 1 ? values[1] : -1;
  switch (profile_idc) {
    case 0:
      *profile = AV1PROFILE_PROFILE_MAIN;
      break;
    case 1:
      *profile = AV1PROFILE_PROFILE_HIGH;
      break;
    case 2:
      *profile = AV1PROFILE_PROFILE_PRO;
      break;
    default:
      DVLOG(3) << __func__ << " Invalid profile (" << fields[1] << ")";
      return false;
  }

  // The level parameter value SHALL equal the first level value indicated by
  // seq_level_idx in the Sequence Header. Note: We validate that this field has
  // the required leading zeros above.
  *level_idc = values[2];
  if (*level_idc > 31) {
    DVLOG(3) << __func__ << " Invalid level (" << *level_idc << ")";
    return false;
  }

  // The bitDepth parameter value SHALL equal the value of BitDepth variable as
  // defined in [AV1] derived from the Sequence Header. Leading zeros required.
  const int bit_depth = values[3];
  if (fields[3].size() != 2 ||
      (bit_depth != 8 && bit_depth != 10 && bit_depth != 12)) {
    DVLOG(3) << __func__ << " Invalid bit-depth (" << fields[3] << ")";
    return false;
  }

  if (values.size() <= 4) return true;

  // The monochrome parameter value, represented by a single digit decimal,
  // SHALL equal the value of mono_chrome in the Sequence Header.
  const int monochrome = values[4];
  if (fields[4].size() != 1 || monochrome > 1) {
    DVLOG(3) << __func__ << " Invalid monochrome (" << fields[4] << ")";
    return false;
  }

  if (values.size() <= 5) return true;

  // The chromaSubsampling parameter value, represented by a three-digit
  // decimal, SHALL have its first digit equal to subsampling_x and its second
  // digit equal to subsampling_y. If both subsampling_x and subsampling_y are
  // set to 1, then the third digit SHALL be equal to chroma_sample_position,
  // otherwise it SHALL be set to 0.
  if (fields[5].size() != 3) {
    DVLOG(3) << __func__ << " Invalid chroma subsampling (" << fields[5] << ")";
    return false;
  }

  const char subsampling_x = fields[5][0];
  const char subsampling_y = fields[5][1];
  const char chroma_sample_position = fields[5][2];
  if ((subsampling_x < '0' || subsampling_x > '1') ||
      (subsampling_y < '0' || subsampling_y > '1') ||
      (chroma_sample_position < '0' || chroma_sample_position > '3')) {
    DVLOG(3) << __func__ << " Invalid chroma subsampling (" << fields[5] << ")";
    return false;
  }

  if (((subsampling_x == '0' || subsampling_y == '0') &&
       chroma_sample_position != '0')) {
    DVLOG(3) << __func__ << " Invalid chroma subsampling (" << fields[5] << ")";
    return false;
  }

  if (values.size() <= 6) return true;

  // The colorPrimaries, transferCharacteristics, matrixCoefficients and
  // videoFullRangeFlag parameter values SHALL equal the value of matching
  // fields in the Sequence Header, if color_description_present_flag is set to
  // 1, otherwise they SHOULD not be set, defaulting to the values below. The
  // videoFullRangeFlag is represented by a single digit.
  auto primaries = gfx::CbColorSpace::PrimaryIDFromInt(values[6]);
  if (fields[6].size() != 2 ||
      primaries == gfx::CbColorSpace::kPrimaryIdReserved0) {
    DVLOG(3) << __func__ << " Invalid color primaries (" << fields[6] << ")";
    return false;
  }

  if (values.size() <= 7) return true;

  auto transfer = gfx::CbColorSpace::TransferIDFromInt(values[7]);
  if (fields[7].size() != 2 ||
      transfer == gfx::CbColorSpace::kTransferIdReserved0) {
    DVLOG(3) << __func__ << " Invalid transfer function (" << fields[7] << ")";
    return false;
  }

  if (values.size() <= 8) return true;

  auto matrix = gfx::CbColorSpace::MatrixIDFromInt(values[8]);
  if (fields[8].size() != 2 || matrix == gfx::CbColorSpace::kMatrixIdUnknown) {
    // TODO: AV1 allows a few matrices we don't support yet.
    //       https://crbug.com/854290
    if (values[8] == 12 || values[8] == 13 || values[8] == 14) {
      DVLOG(3) << __func__ << " Unsupported matrix coefficients (" << fields[8]
               << ")";
    } else {
      DVLOG(3) << __func__ << " Invalid matrix coefficients (" << fields[8]
               << ")";
    }
    return false;
  }

  if (values.size() <= 9) return true;

  const int video_full_range_flag = values[9];
  if (fields[9].size() != 1 || video_full_range_flag > 1) {
    DVLOG(3) << __func__ << " Invalid full range flag (" << fields[9] << ")";
    return false;
  }
  auto range = video_full_range_flag == 1 ? gfx::CbColorSpace::kRangeIdFull
                                          : gfx::CbColorSpace::kRangeIdLimited;
  *color_space = gfx::CbColorSpace(primaries, transfer, matrix, range);
  return true;
}

bool ParseAVCCodecId(const std::string& codec_id, VideoCodecProfile* profile,
                     uint8_t* level_idc) {
  // Make sure we have avc1.xxxxxx or avc3.xxxxxx , where xxxxxx are hex digits
  if (!StartsWith(codec_id, "avc1.", base::CompareCase::SENSITIVE) &&
      !StartsWith(codec_id, "avc3.", base::CompareCase::SENSITIVE)) {
    return false;
  }
  uint32_t elem = 0;
  if (codec_id.size() != 11 ||
      !base::HexStringToUInt(base::StringPiece(codec_id).substr(5), &elem)) {
    DVLOG(4) << __func__ << ": invalid avc codec id (" << codec_id << ")";
    return false;
  }

  uint8_t level_byte = elem & 0xFF;
  uint8_t constraints_byte = (elem >> 8) & 0xFF;
  uint8_t profile_idc = (elem >> 16) & 0xFF;

  // Check that the lower two bits of |constraints_byte| are zero (those are
  // reserved and must be zero according to ISO IEC 14496-10).
  if (constraints_byte & 3) {
    DVLOG(4) << __func__ << ": non-zero reserved bits in codec id " << codec_id;
    return false;
  }

  VideoCodecProfile out_profile = VIDEO_CODEC_PROFILE_UNKNOWN;
  // profile_idc values for each profile are taken from ISO IEC 14496-10 and
  // https://en.wikipedia.org/wiki/H.264/MPEG-4_AVC#Profiles
  switch (profile_idc) {
    case 66:
      out_profile = H264PROFILE_BASELINE;
      break;
    case 77:
      out_profile = H264PROFILE_MAIN;
      break;
    case 83:
      out_profile = H264PROFILE_SCALABLEBASELINE;
      break;
    case 86:
      out_profile = H264PROFILE_SCALABLEHIGH;
      break;
    case 88:
      out_profile = H264PROFILE_EXTENDED;
      break;
    case 100:
      out_profile = H264PROFILE_HIGH;
      break;
    case 110:
      out_profile = H264PROFILE_HIGH10PROFILE;
      break;
    case 118:
      out_profile = H264PROFILE_MULTIVIEWHIGH;
      break;
    case 122:
      out_profile = H264PROFILE_HIGH422PROFILE;
      break;
    case 128:
      out_profile = H264PROFILE_STEREOHIGH;
      break;
    case 244:
      out_profile = H264PROFILE_HIGH444PREDICTIVEPROFILE;
      break;
    default:
      DVLOG(1) << "Warning: unrecognized AVC/H.264 profile " << profile_idc;
      return false;
  }

  // TODO(servolk): Take into account also constraint set flags 3 through 5.
  uint8_t constraint_set0_flag = (constraints_byte >> 7) & 1;
  uint8_t constraint_set1_flag = (constraints_byte >> 6) & 1;
  uint8_t constraint_set2_flag = (constraints_byte >> 5) & 1;
  if (constraint_set2_flag && out_profile > H264PROFILE_EXTENDED) {
    out_profile = H264PROFILE_EXTENDED;
  }
  if (constraint_set1_flag && out_profile > H264PROFILE_MAIN) {
    out_profile = H264PROFILE_MAIN;
  }
  if (constraint_set0_flag && out_profile > H264PROFILE_BASELINE) {
    out_profile = H264PROFILE_BASELINE;
  }

  if (level_idc) *level_idc = level_byte;

  if (profile) *profile = out_profile;

  return true;
}

// The specification for HEVC codec id strings can be found in ISO IEC 14496-15
// dated 2012 or newer in the Annex E.3
bool ParseHEVCCodecId(const std::string& codec_id, VideoCodecProfile* profile,
                      uint8_t* level_idc) {
  if (!StartsWith(codec_id, "hev1.", base::CompareCase::SENSITIVE) &&
      !StartsWith(codec_id, "hvc1.", base::CompareCase::SENSITIVE)) {
    return false;
  }

  // HEVC codec id consists of:
  const int kMaxHevcCodecIdLength =
      5 +  // 'hev1.' or 'hvc1.' prefix (5 chars)
      4 +  // profile, e.g. '.A12' (max 4 chars)
      9 +  // profile_compatibility, dot + 32-bit hex number (max 9 chars)
      5 +  // tier and level, e.g. '.H120' (max 5 chars)
      18;  // up to 6 constraint bytes, bytes are dot-separated and hex-encoded.

  if (codec_id.size() > kMaxHevcCodecIdLength) {
    DVLOG(4) << __func__ << ": Codec id is too long (" << codec_id << ")";
    return false;
  }

  std::vector<std::string> elem = base::SplitString(
      codec_id, std::string("."), base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  DCHECK(elem[0] == "hev1" || elem[0] == "hvc1");

  if (elem.size() < 4) {
    DVLOG(4) << __func__ << ": invalid HEVC codec id " << codec_id;
    return false;
  }

  uint8_t general_profile_space = 0;
  if (elem[1].size() > 0 &&
      (elem[1][0] == 'A' || elem[1][0] == 'B' || elem[1][0] == 'C')) {
    general_profile_space = 1 + (elem[1][0] - 'A');
    elem[1].erase(0, 1);
  }
  DCHECK(general_profile_space >= 0 && general_profile_space <= 3);

  unsigned general_profile_idc = 0;
  if (!base::StringToUint(elem[1], &general_profile_idc) ||
      general_profile_idc > 0x1f) {
    DVLOG(4) << __func__ << ": invalid general_profile_idc=" << elem[1];
    return false;
  }

  uint32_t general_profile_compatibility_flags = 0;
  if (!base::HexStringToUInt(elem[2], &general_profile_compatibility_flags)) {
    DVLOG(4) << __func__
             << ": invalid general_profile_compatibility_flags=" << elem[2];
    return false;
  }

  if (profile) {
    // TODO(servolk): Handle format range extension profiles as explained in
    // HEVC standard (ISO/IEC ISO/IEC 23008-2) section A.3.5
    if (general_profile_idc == 3 || (general_profile_compatibility_flags & 4)) {
      *profile = HEVCPROFILE_MAIN_STILL_PICTURE;
    }
    if (general_profile_idc == 2 || (general_profile_compatibility_flags & 2)) {
      *profile = HEVCPROFILE_MAIN10;
    }
    if (general_profile_idc == 1 || (general_profile_compatibility_flags & 1)) {
      *profile = HEVCPROFILE_MAIN;
    }
  }

  uint8_t general_tier_flag;
  if (elem[3].size() > 0 && (elem[3][0] == 'L' || elem[3][0] == 'H')) {
    general_tier_flag = (elem[3][0] == 'L') ? 0 : 1;
    elem[3].erase(0, 1);
  } else {
    DVLOG(4) << __func__ << ": invalid general_tier_flag=" << elem[3];
    return false;
  }
  DCHECK(general_tier_flag == 0 || general_tier_flag == 1);

  unsigned general_level_idc = 0;
  if (!base::StringToUint(elem[3], &general_level_idc) ||
      general_level_idc > 0xff) {
    DVLOG(4) << __func__ << ": invalid general_level_idc=" << elem[3];
    return false;
  }

  if (level_idc) *level_idc = static_cast<uint8_t>(general_level_idc);

  uint8_t constraint_flags[6];
  SbMemorySet(constraint_flags, 0, sizeof(constraint_flags));

  if (elem.size() > 10) {
    DVLOG(4) << __func__ << ": unexpected number of trailing bytes in HEVC "
             << "codec id " << codec_id;
    return false;
  }
  for (size_t i = 4; i < elem.size(); ++i) {
    unsigned constr_byte = 0;
    if (!base::HexStringToUInt(elem[i], &constr_byte) || constr_byte > 0xFF) {
      DVLOG(4) << __func__ << ": invalid constraint byte=" << elem[i];
      return false;
    }
    constraint_flags[i - 4] = constr_byte;
  }

  return true;
}

VideoCodec StringToVideoCodec(const std::string& codec_id) {
  std::vector<std::string> elem = base::SplitString(
      codec_id, std::string("."), base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  if (elem.empty()) return kUnknownVideoCodec;
  VideoCodecProfile profile = VIDEO_CODEC_PROFILE_UNKNOWN;
  uint8_t level = 0;
  if (ParseAVCCodecId(codec_id, &profile, &level)) return kCodecH264;
  if (codec_id == "vp8" || codec_id == "vp8.0") return kCodecVP8;
  if (codec_id == "vp9" || codec_id == "vp9.0" || codec_id == "vp9.1" ||
      codec_id == "vp9.2") {
    return kCodecVP9;
  }
  gfx::CbColorSpace color_space;
  if (ParseAv1CodecId(codec_id, &profile, &level, &color_space))
    return kCodecAV1;
  if (codec_id == "theora") return kCodecTheora;
  if (ParseHEVCCodecId(codec_id, &profile, &level)) return kCodecHEVC;

  return kUnknownVideoCodec;
}

}  // namespace media
}  // namespace cobalt
