// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <dt-bindings/media/qcom,glymur-iris.h>
#include <linux/iris_vpu_bus.h>

#include "iris_core.h"
#include "iris_platform_common.h"
#include "iris_platform_glymur.h"

const struct iris_power_domain_data iris_glymur_ctrl_data = {
	.pd_names = (const char *[]) {
		"venus",
	},
	.pd_cnt = 1,
	.clk_names = (const char *[]) {
		"core_iface", "core_freerun", "core",
	},
	.clk_cnt = 3,
};

const struct iris_power_domain_data iris_glymur_vcodec_data[] = {
	{
		.pd_names = (const char *[]) {
			"vcodec0",
		},
		.pd_cnt = 1,
		.clk_names = (const char *[]) {
			"vcodec0_iface", "vcodec0_core_freerun", "vcodec0_core",
		},
		.clk_cnt = 3,
	},
	{
		.pd_names = (const char *[]) {
			"vcodec1",
		},
		.pd_cnt = 1,
		.clk_names = (const char *[]) {
			"vcodec1_iface", "vcodec1_core_freerun", "vcodec1_core",
		},
		.clk_cnt = 3,
	},
};

const char * const iris_glymur_clk_reset_table[] = {
	"core_bus",
	"vcodec0_bus",
	"core",
	"vcodec0_core",
	"vcodec1_bus",
	"vcodec1_core",
};

const char * const iris_glymur_opp_clk_table[] = {
	"vcodec0_core",
	"vcodec1_core",
	"core",
	NULL,
};

const struct tz_cp_config iris_glymur_tz_cp_config[] = {
	{
		.cp_start = VIDEO_REGION_SECURE_FW_REGION_ID,
		.cp_size = 0,
		.cp_nonpixel_start = 0,
		.cp_nonpixel_size = 0x1000000,
	},
	{
		.cp_start = VIDEO_REGION_VM0_SECURE_NP_ID,
		.cp_size = 0,
		.cp_nonpixel_start = 0x1000000,
		.cp_nonpixel_size = 0x24800000,
	},
	{
		.cp_start = VIDEO_REGION_VM0_NONSECURE_NP_ID,
		.cp_size = 0,
		.cp_nonpixel_start = 0x25800000,
		.cp_nonpixel_size = 0xda600000,
	},
};

int iris_glymur_init_cb_devs(struct iris_core *core)
{
	u64 dma_mask = core->iris_platform_data->dma_mask;
	const u32 fw_fid = IOMMU_FID_IRIS_FIRMWARE;
	struct device *dev;

	dev = iris_vpu_bus_create_device(core->dev, "iris-firmware", dma_mask, &fw_fid);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	if (device_iommu_mapped(dev))
		core->fw_dev = dev;
	else
		device_unregister(dev);

	return 0;
}

void iris_glymur_deinit_cb_devs(struct iris_core *core)
{
	if (core->fw_dev)
		device_unregister(core->fw_dev);
}
