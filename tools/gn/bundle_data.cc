// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tools/gn/bundle_data.h"

#include "base/logging.h"
#include "tools/gn/output_file.h"
#include "tools/gn/settings.h"
#include "tools/gn/target.h"

namespace {

// Return directory of |path| without the trailing directory separator.
base::StringPiece FindDirNoTrailingSeparator(const base::StringPiece& path) {
  base::StringPiece::size_type pos = path.find_last_of("/\\");
  if (pos == base::StringPiece::npos)
    return base::StringPiece();
  return base::StringPiece(path.data(), pos);
}

}  // namespace

bool IsSourceFileFromAssetCatalog(const SourceFile& source,
                                  SourceFile* asset_catalog) {
  // Check that the file matches the following pattern:
  //    .*\.xcassets/[^/]*\.imageset/[^/]*
  base::StringPiece dir;
  dir = FindDirNoTrailingSeparator(source.value());
  if (!dir.ends_with(".imageset"))
    return false;
  dir = FindDirNoTrailingSeparator(dir);
  if (!dir.ends_with(".xcassets"))
    return false;
  if (asset_catalog) {
    std::string asset_catalog_path = dir.as_string();
    *asset_catalog = SourceFile(SourceFile::SWAP_IN, &asset_catalog_path);
  }
  return true;
}

BundleData::BundleData() {}

BundleData::~BundleData() {}

void BundleData::AddFileRuleFromTarget(const Target* target) {
  DCHECK_EQ(target->output_type(), Target::BUNDLE_DATA);

  std::vector<SourceFile> file_rule_sources;
  for (const SourceFile& source_file : target->sources()) {
    if (IsSourceFileFromAssetCatalog(source_file, nullptr)) {
      asset_catalog_sources_.push_back(source_file);
    } else {
      file_rule_sources.push_back(source_file);
    }
  }

  if (!file_rule_sources.empty()) {
    DCHECK_EQ(target->action_values().outputs().list().size(), 1u);
    file_rules_.push_back(BundleFileRule(
        file_rule_sources, target->action_values().outputs().list()[0]));
  }
}

void BundleData::GetSourceFiles(std::vector<SourceFile>* sources) const {
  for (const BundleFileRule& file_rule : file_rules_) {
    sources->insert(sources->end(), file_rule.sources().begin(),
                    file_rule.sources().end());
  }
  sources->insert(sources->end(), asset_catalog_sources_.begin(),
                  asset_catalog_sources_.end());
}

void BundleData::GetOutputFiles(const Settings* settings,
                                std::vector<OutputFile>* outputs) const {
  std::vector<SourceFile> outputs_as_sources;
  GetOutputsAsSourceFiles(settings, &outputs_as_sources);
  for (const SourceFile& source_file : outputs_as_sources)
    outputs->push_back(OutputFile(settings->build_settings(), source_file));
}

void BundleData::GetOutputsAsSourceFiles(
    const Settings* settings,
    std::vector<SourceFile>* outputs_as_source) const {
  for (const BundleFileRule& file_rule : file_rules_) {
    for (const SourceFile& source : file_rule.sources()) {
      outputs_as_source->push_back(
          file_rule.ApplyPatternToSource(settings, *this, source));
    }
  }

  if (!asset_catalog_sources_.empty())
    outputs_as_source->push_back(GetCompiledAssetCatalogPath());
}

SourceFile BundleData::GetCompiledAssetCatalogPath() const {
  DCHECK(!asset_catalog_sources_.empty());
  std::string assets_car_path = resources_dir_ + "/Assets.car";
  return SourceFile(SourceFile::SWAP_IN, &assets_car_path);
}
