/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ultralight_java/gpudriver/common/GPUDriverImpl.h"

namespace ultralight {

    GPUDriverImpl::GPUDriverImpl() : batch_count_(0) {
    }

    GPUDriverImpl::~GPUDriverImpl() {
    }

    bool GPUDriverImpl::HasCommandsPending() {
        return !command_list_.empty();
    }

    void GPUDriverImpl::DrawCommandList() {
        if(command_list_.empty())
            return;

        batch_count_ = 0;

        for(auto &cmd : command_list_) {
            if(cmd.command_type == kCommandType_DrawGeometry)
                DrawGeometry(cmd.geometry_id, cmd.indices_count, cmd.indices_offset, cmd.gpu_state);
            else if(cmd.command_type == kCommandType_ClearRenderBuffer)
                ClearRenderBuffer(cmd.gpu_state.render_buffer_id);
            batch_count_++;
        }

        command_list_.clear();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int GPUDriverImpl::batch_count() const {
        return batch_count_;
    }

    void GPUDriverImpl::BeginSynchronize() {
    }

    void GPUDriverImpl::EndSynchronize() {
    }

    uint32_t GPUDriverImpl::NextTextureId() {
        return next_texture_id_++;
    }

    uint32_t GPUDriverImpl::NextRenderBufferId() {
        return next_render_buffer_id_++;
    }

    uint32_t GPUDriverImpl::NextGeometryId() {
        return next_geometry_id_++;
    }

    void GPUDriverImpl::UpdateCommandList(const CommandList &list) {
        if(list.size) {
            command_list_.resize(list.size);
            memcpy(&command_list_[0], list.commands, sizeof(Command) * list.size);
        }
    }

} // namespace ultralight