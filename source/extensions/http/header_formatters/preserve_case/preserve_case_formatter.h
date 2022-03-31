#pragma once

#include "envoy/http/header_formatter.h"

#include "common/common/utility.h"
#include "common/http/http1/header_formatter.h"

namespace Envoy {
namespace Extensions {
namespace Http {
namespace HeaderFormatters {
namespace PreserveCase {

class PreserveCaseHeaderFormatter : public Envoy::Http::StatefulHeaderKeyFormatter {
public:
  // Envoy::Http::StatefulHeaderKeyFormatter
  std::string format(absl::string_view key) const override;
  void processKey(absl::string_view key) override;

private:
  StringUtil::CaseUnorderedSet original_header_keys_;
  Envoy::Http::Http1::ProperCaseHeaderKeyFormatter proper_case_header_key_formatter_;
};

} // namespace PreserveCase
} // namespace HeaderFormatters
} // namespace Http
} // namespace Extensions
} // namespace Envoy
