#pragma once

#include <Windows.h>
#include <vector>
#include <cstdint>

class mem_patch
{
public:
	mem_patch(uintptr_t address, std::vector<uint8_t> patch_bytes)
	{
		m_patch_addr = address;
		m_patch_bytes = patch_bytes;
		m_is_patched = false;
		m_orig_bytes.clear();
	}
	void patch()
	{
		if (!m_is_patched)
		{
			if (!m_orig_bytes.size())
			{
				m_orig_bytes.resize(m_patch_bytes.size());
				memcpy(m_orig_bytes.data(), (void*)m_patch_addr, m_patch_bytes.size());
			}

			DWORD old_protect;
			if (VirtualProtect((void*)m_patch_addr, m_orig_bytes.size(), PAGE_EXECUTE_READWRITE, &old_protect))
			{
				memcpy((void*)m_patch_addr, m_patch_bytes.data(), m_patch_bytes.size());
				VirtualProtect((void*)m_patch_addr, m_orig_bytes.size(), old_protect, &old_protect);
				m_is_patched = true;
			}
		}
	}
	void restore()
	{
		if (m_is_patched && m_orig_bytes.size())
		{
			DWORD old_protect;
			if (VirtualProtect((void*)m_patch_addr, m_orig_bytes.size(), PAGE_EXECUTE_READWRITE, &old_protect))
			{
				memcpy((void*)m_patch_addr, m_orig_bytes.data(), m_orig_bytes.size());
				VirtualProtect((void*)m_patch_addr, m_orig_bytes.size(), old_protect, &old_protect);
				m_is_patched = false;
			}
		}
	}
private:
	std::vector<uint8_t>	m_orig_bytes;
	std::vector<uint8_t>	m_patch_bytes;
	bool					m_is_patched;
	uintptr_t				m_patch_addr;
};