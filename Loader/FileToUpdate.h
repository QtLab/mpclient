#pragma once

#include "Defs.h"

#include <string>
#include <vector>

#include "json/json.h"

namespace ldr {

class FileToUpdate;
typedef FileToUpdate* FileToUpdatePtr;
typedef std::vector<FileToUpdatePtr> FilesToUpdate;

class FileToUpdate
{
public:
	FileToUpdate(){}

	void SetUrl(const String& url);
	void SetRelativePath(const String& relativePath);
	void SetMD5(const String& md5);

	const String& Domain() const;
	const String& Query() const;
	const String& Url() const;
	const String& RelativePath() const;
	const String& AbsolutePath() const;
	String AbdoluteDirectory() const;
	const String& MD5() const;

	bool Exists() const;

	static FileToUpdatePtr CreateFromJsonValue(const Json::Value& value);
	static bool FillFromJsonValue(FileToUpdatePtr fileToUpdate, const Json::Value& value);
private:
	String		m_domain;
	String		m_query;
	String		m_url;
	String		m_relativePath;
	String		m_absolutePath;
	String		m_md5;
};



}