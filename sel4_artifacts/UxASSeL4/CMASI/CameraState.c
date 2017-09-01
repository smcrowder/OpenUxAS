
#include "common/struct_defines.h"
#include "common/conv.h"
#include "CameraState.h"
#include "GimballedPayloadState.h"
#include "Location3D.h"
#include "Location3D.h"
void lmcp_pp_CameraState(CameraState* s) {
    printf("CameraState{");
    printf("Inherited from GimballedPayloadState:\n");
    lmcp_pp_GimballedPayloadState(&(s->super));
    printf("horizontalfieldofview: ");
    printf("%u",s->horizontalfieldofview);
    printf("\n");
    printf("verticalfieldofview: ");
    printf("%u",s->verticalfieldofview);
    printf("\n");
    printf("footprint: ");
    printf("[");
    for (uint32_t index = 0; index < s->footprint_ai.length; index++) {
        lmcp_pp_Location3D((s->footprint[index]));
        printf(",");
    }
    printf("\n");
    printf("centerpoint: ");
    lmcp_pp_Location3D((s->centerpoint));
    printf("\n");
    printf("}");
}
size_t lmcp_packsize_CameraState (CameraState* i) {
    size_t out = 0;
    out += lmcp_packsize_GimballedPayloadState(&(i->super));
    out += sizeof(uint32_t);
    out += sizeof(uint32_t);
    out += 2;
    for (uint32_t index = 0; index < i->footprint_ai.length; index++) {
        out += 15 + lmcp_packsize_Location3D(i->footprint[index]);
    }
    if (i->centerpoint==NULL) {
        out += 1;
    } else {
        out += 15 + lmcp_packsize_Location3D(i->centerpoint);
    }
    return out;
}
size_t lmcp_pack_CameraState_header(uint8_t* buf, CameraState* i) {
    uint8_t* outb = buf;
    if (i == NULL) {
        lmcp_pack_uint8_t(outb, 0);
        return 1;
    }
    outb += lmcp_pack_uint8_t(outb, 1);
    memcpy(outb, "CMASI", 5);
    outb += 5;
    for (size_t j = 5; j < 8; j++, outb++)
        *outb = 0;
    outb += lmcp_pack_uint32_t(outb, 21);
    outb += lmcp_pack_uint16_t(outb, 3);
    return 15;
}
void lmcp_free_CameraState(CameraState* out, int out_malloced) {
    if (out == NULL)
        return;
    lmcp_free_GimballedPayloadState(&(out->super), 0);
    if (out->footprint != NULL) {
        for (uint32_t index = 0; index < out->footprint_ai.length; index++) {
            if (out->footprint[index] != NULL) {
                lmcp_free_Location3D(out->footprint[index], 1);
            }
        }
        free(out->footprint);
    }
    if (out->centerpoint != NULL) {
        lmcp_free_Location3D(out->centerpoint, 1);
    }
    if (out_malloced == 1) {
        free(out);
    }
}
void lmcp_init_CameraState (CameraState** i) {
    if (i == NULL) return;
    (*i) = calloc(1,sizeof(CameraState));
    ((lmcp_object*)(*i)) -> type = 21;
}
int lmcp_unpack_CameraState(uint8_t** inb, size_t *size_remain, CameraState* outp) {
    if (inb == NULL || *inb == NULL) {
        return -1;
    }
    if (size_remain == NULL || *size_remain == 0) {
        return -1;
    }
    CameraState* out = outp;
    uint32_t tmp;
    uint16_t tmp16;
    CHECK(lmcp_unpack_GimballedPayloadState(inb, size_remain, &(out->super)))
    CHECK(lmcp_unpack_uint32_t(inb, size_remain, &(out->horizontalfieldofview)))
    CHECK(lmcp_unpack_uint32_t(inb, size_remain, &(out->verticalfieldofview)))
    CHECK(lmcp_unpack_uint16_t(inb, size_remain, &tmp16))
    tmp = tmp16;
    (out)->footprint = malloc(sizeof(Location3D*) * tmp);
    if (out->footprint==0) {
        return -1;
    }
    out->footprint_ai.length = tmp;
    for (uint32_t index = 0; index < out->footprint_ai.length; index++) {
        uint8_t isnull;
        uint32_t objtype;
        uint16_t objseries;
        char seriesname[8];
        CHECK(lmcp_unpack_uint8_t(inb, size_remain, &isnull))
        if (isnull == 0 && inb != NULL) {
            out->footprint[index] = NULL;
        } else if (inb != NULL) {
            CHECK(lmcp_unpack_8byte(inb, size_remain, seriesname))
            CHECK(lmcp_unpack_uint32_t(inb, size_remain, &objtype))
            CHECK(lmcp_unpack_uint16_t(inb, size_remain, &objseries))
            lmcp_init_Location3D(&(out->footprint[index]));
            CHECK(lmcp_unpack_Location3D(inb, size_remain, (out->footprint[index])))
        }
    }
    uint8_t isnull;
    uint32_t objtype;
    uint16_t objseries;
    char seriesname[8];
    CHECK(lmcp_unpack_uint8_t(inb, size_remain, &isnull))
    if (isnull == 0 && inb != NULL) {
        out->centerpoint = NULL;
    } else if (inb != NULL) {
        CHECK(lmcp_unpack_8byte(inb, size_remain, seriesname))
        CHECK(lmcp_unpack_uint32_t(inb, size_remain, &objtype))
        CHECK(lmcp_unpack_uint16_t(inb, size_remain, &objseries))
        lmcp_init_Location3D(&(out->centerpoint));
        CHECK(lmcp_unpack_Location3D(inb, size_remain, (out->centerpoint)))
    }
    return 0;
}
size_t lmcp_pack_CameraState(uint8_t* buf, CameraState* i) {
    if (i == NULL) return 0;
    uint8_t* outb = buf;
    outb += lmcp_pack_GimballedPayloadState(outb, &(i->super));
    outb += lmcp_pack_uint32_t(outb, i->horizontalfieldofview);
    outb += lmcp_pack_uint32_t(outb, i->verticalfieldofview);
    outb += lmcp_pack_uint16_t(outb, i->footprint_ai.length);
    for (uint32_t index = 0; index < i->footprint_ai.length; index++) {
        if (i->footprint[index]==NULL) {
            outb += lmcp_pack_uint8_t(outb, 0);
        } else {
            outb += lmcp_pack_uint8_t(outb, 1);
            memcpy(outb, "CMASI", 5);
            outb += 5;
            for (size_t j = 5; j < 8; j++, outb++)
                *outb = 0;
            outb += lmcp_pack_uint32_t(outb, 3);
            outb += lmcp_pack_uint16_t(outb, 3);
            outb += lmcp_pack_Location3D(outb, i->footprint[index]);
        }
    }
    if (i->centerpoint==NULL) {
        outb += lmcp_pack_uint8_t(outb, 0);
    } else {
        outb += lmcp_pack_uint8_t(outb, 1);
        memcpy(outb, "CMASI", 5);
        outb += 5;
        for (size_t j = 5; j < 8; j++, outb++)
            *outb = 0;
        outb += lmcp_pack_uint32_t(outb, 3);
        outb += lmcp_pack_uint16_t(outb, 3);
        outb += lmcp_pack_Location3D(outb, i->centerpoint);
    }
    return (outb - buf);
}
