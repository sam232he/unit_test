/**
 * @file    spi_flash_w25q128jv.h
 *
 * @author  Amirhossein Safari
 *
 * @date    07.07.2021 (date of creation)
 *
 * @ingroup INCLUDE BMODULE
 *
 * @prefix  spi_flash_w25q128jv
 *
 * @brief   spi_flash_w25q128jv header file
 */

#ifndef SPI_FLASH_W25Q128JV_H
#define SPI_FLASH_W25Q128JV_H

/*================= Includes ===============================================*/
#include "subm_mms/include/mms_common.h"
/*================== Macros and Definitions =================================*/


#define SPI_FLASH_W25Q128JV_MAX_BLOCK (255U)

typedef enum
{
    spi_flash_w25q128jv_block0 = (0U),
    spi_flash_w25q128jv_block1 = (1U),
    spi_flash_w25q128jv_block2 = (2U),
    spi_flash_w25q128jv_block3 = (3U),
    spi_flash_w25q128jv_block4 = (4U),
    spi_flash_w25q128jv_block5 = (5U),
    spi_flash_w25q128jv_block6 = (6U),
    spi_flash_w25q128jv_block7 = (7U),
    spi_flash_w25q128jv_block8 = (8U),
    spi_flash_w25q128jv_block9 = (9U),
    spi_flash_w25q128jv_block10 = (10U),
    spi_flash_w25q128jv_block11 = (11U),
    spi_flash_w25q128jv_block12 = (12U),
    spi_flash_w25q128jv_block13 = (13U),
    spi_flash_w25q128jv_block14 = (14U),
    spi_flash_w25q128jv_block15 = (15U),
    spi_flash_w25q128jv_block16 = (16U),
    spi_flash_w25q128jv_block17 = (17U),
    spi_flash_w25q128jv_block18 = (18U),
    spi_flash_w25q128jv_block19 = (19U),
    spi_flash_w25q128jv_block20 = (20U),
    spi_flash_w25q128jv_block21 = (21U),
    spi_flash_w25q128jv_block22 = (22U),
    spi_flash_w25q128jv_block23 = (23U),
    spi_flash_w25q128jv_block24 = (24U),
    spi_flash_w25q128jv_block25 = (25U),
    spi_flash_w25q128jv_block26 = (26U),
    spi_flash_w25q128jv_block27 = (27U),
    spi_flash_w25q128jv_block28 = (28U),
    spi_flash_w25q128jv_block29 = (29U),
    spi_flash_w25q128jv_block30 = (30U),
    spi_flash_w25q128jv_block31 = (31U),
    spi_flash_w25q128jv_block32 = (32U),
    spi_flash_w25q128jv_block33 = (33U),
    spi_flash_w25q128jv_block34 = (34U),
    spi_flash_w25q128jv_block35 = (35U),
    spi_flash_w25q128jv_block36 = (36U),
    spi_flash_w25q128jv_block37 = (37U),
    spi_flash_w25q128jv_block38 = (38U),
    spi_flash_w25q128jv_block39 = (39U),
    spi_flash_w25q128jv_block40 = (40U),
    spi_flash_w25q128jv_block41 = (41U),
    spi_flash_w25q128jv_block42 = (42U),
    spi_flash_w25q128jv_block43 = (43U),
    spi_flash_w25q128jv_block44 = (44U),
    spi_flash_w25q128jv_block45 = (45U),
    spi_flash_w25q128jv_block46 = (46U),
    spi_flash_w25q128jv_block47 = (47U),
    spi_flash_w25q128jv_block48 = (48U),
    spi_flash_w25q128jv_block49 = (49U),
    spi_flash_w25q128jv_block50 = (50U),
    spi_flash_w25q128jv_block51 = (51U),
    spi_flash_w25q128jv_block52 = (52U),
    spi_flash_w25q128jv_block53 = (53U),
    spi_flash_w25q128jv_block54 = (54U),
    spi_flash_w25q128jv_block55 = (55U),
    spi_flash_w25q128jv_block56 = (56U),
    spi_flash_w25q128jv_block57 = (57U),
    spi_flash_w25q128jv_block58 = (58U),
    spi_flash_w25q128jv_block59 = (59U),
    spi_flash_w25q128jv_block60 = (60U),
    spi_flash_w25q128jv_block61 = (61U),
    spi_flash_w25q128jv_block62 = (62U),
    spi_flash_w25q128jv_block63 = (63U),
    spi_flash_w25q128jv_block64 = (64U),
    spi_flash_w25q128jv_block65 = (65U),
    spi_flash_w25q128jv_block66 = (66U),
    spi_flash_w25q128jv_block67 = (67U),
    spi_flash_w25q128jv_block68 = (68U),
    spi_flash_w25q128jv_block69 = (69U),
    spi_flash_w25q128jv_block70 = (70U),
    spi_flash_w25q128jv_block71 = (71U),
    spi_flash_w25q128jv_block72 = (72U),
    spi_flash_w25q128jv_block73 = (73U),
    spi_flash_w25q128jv_block74 = (74U),
    spi_flash_w25q128jv_block75 = (75U),
    spi_flash_w25q128jv_block76 = (76U),
    spi_flash_w25q128jv_block77 = (77U),
    spi_flash_w25q128jv_block78 = (78U),
    spi_flash_w25q128jv_block79 = (79U),
    spi_flash_w25q128jv_block80 = (80U),
    spi_flash_w25q128jv_block81 = (81U),
    spi_flash_w25q128jv_block82 = (82U),
    spi_flash_w25q128jv_block83 = (83U),
    spi_flash_w25q128jv_block84 = (84U),
    spi_flash_w25q128jv_block85 = (85U),
    spi_flash_w25q128jv_block86 = (86U),
    spi_flash_w25q128jv_block87 = (87U),
    spi_flash_w25q128jv_block88 = (88U),
    spi_flash_w25q128jv_block89 = (89U),
    spi_flash_w25q128jv_block90 = (90U),
    spi_flash_w25q128jv_block91 = (91U),
    spi_flash_w25q128jv_block92 = (92U),
    spi_flash_w25q128jv_block93 = (93U),
    spi_flash_w25q128jv_block94 = (94U),
    spi_flash_w25q128jv_block95 = (95U),
    spi_flash_w25q128jv_block96 = (96U),
    spi_flash_w25q128jv_block97 = (97U),
    spi_flash_w25q128jv_block98 = (98U),
    spi_flash_w25q128jv_block99 = (99U),
    spi_flash_w25q128jv_block100 = (100U),
    spi_flash_w25q128jv_block101 = (101U),
    spi_flash_w25q128jv_block102 = (102U),
    spi_flash_w25q128jv_block103 = (103U),
    spi_flash_w25q128jv_block104 = (104U),
    spi_flash_w25q128jv_block105 = (105U),
    spi_flash_w25q128jv_block106 = (106U),
    spi_flash_w25q128jv_block107 = (107U),
    spi_flash_w25q128jv_block108 = (108U),
    spi_flash_w25q128jv_block109 = (109U),
    spi_flash_w25q128jv_block110 = (110U),
    spi_flash_w25q128jv_block111 = (111U),
    spi_flash_w25q128jv_block112 = (112U),
    spi_flash_w25q128jv_block113 = (113U),
    spi_flash_w25q128jv_block114 = (114U),
    spi_flash_w25q128jv_block115 = (115U),
    spi_flash_w25q128jv_block116 = (116U),
    spi_flash_w25q128jv_block117 = (117U),
    spi_flash_w25q128jv_block118 = (118U),
    spi_flash_w25q128jv_block119 = (119U),
    spi_flash_w25q128jv_block120 = (120U),
    spi_flash_w25q128jv_block121 = (121U),
    spi_flash_w25q128jv_block122 = (122U),
    spi_flash_w25q128jv_block123 = (123U),
    spi_flash_w25q128jv_block124 = (124U),
    spi_flash_w25q128jv_block125 = (125U),
    spi_flash_w25q128jv_block126 = (126U),
    spi_flash_w25q128jv_block127 = (127U),
    spi_flash_w25q128jv_block128 = (128U),
    spi_flash_w25q128jv_block129 = (129U),
    spi_flash_w25q128jv_block130 = (130U),
    spi_flash_w25q128jv_block131 = (131U),
    spi_flash_w25q128jv_block132 = (132U),
    spi_flash_w25q128jv_block133 = (133U),
    spi_flash_w25q128jv_block134 = (134U),
    spi_flash_w25q128jv_block135 = (135U),
    spi_flash_w25q128jv_block136 = (136U),
    spi_flash_w25q128jv_block137 = (137U),
    spi_flash_w25q128jv_block138 = (138U),
    spi_flash_w25q128jv_block139 = (139U),
    spi_flash_w25q128jv_block140 = (140U),
    spi_flash_w25q128jv_block141 = (141U),
    spi_flash_w25q128jv_block142 = (142U),
    spi_flash_w25q128jv_block143 = (143U),
    spi_flash_w25q128jv_block144 = (144U),
    spi_flash_w25q128jv_block145 = (145U),
    spi_flash_w25q128jv_block146 = (146U),
    spi_flash_w25q128jv_block147 = (147U),
    spi_flash_w25q128jv_block148 = (148U),
    spi_flash_w25q128jv_block149 = (149U),
    spi_flash_w25q128jv_block150 = (150U),
    spi_flash_w25q128jv_block151 = (151U),
    spi_flash_w25q128jv_block152 = (152U),
    spi_flash_w25q128jv_block153 = (153U),
    spi_flash_w25q128jv_block154 = (154U),
    spi_flash_w25q128jv_block155 = (155U),
    spi_flash_w25q128jv_block156 = (156U),
    spi_flash_w25q128jv_block157 = (157U),
    spi_flash_w25q128jv_block158 = (158U),
    spi_flash_w25q128jv_block159 = (159U),
    spi_flash_w25q128jv_block160 = (160U),
    spi_flash_w25q128jv_block161 = (161U),
    spi_flash_w25q128jv_block162 = (162U),
    spi_flash_w25q128jv_block163 = (163U),
    spi_flash_w25q128jv_block164 = (164U),
    spi_flash_w25q128jv_block165 = (165U),
    spi_flash_w25q128jv_block166 = (166U),
    spi_flash_w25q128jv_block167 = (167U),
    spi_flash_w25q128jv_block168 = (168U),
    spi_flash_w25q128jv_block169 = (169U),
    spi_flash_w25q128jv_block170 = (170U),
    spi_flash_w25q128jv_block171 = (171U),
    spi_flash_w25q128jv_block172 = (172U),
    spi_flash_w25q128jv_block173 = (173U),
    spi_flash_w25q128jv_block174 = (174U),
    spi_flash_w25q128jv_block175 = (175U),
    spi_flash_w25q128jv_block176 = (176U),
    spi_flash_w25q128jv_block177 = (177U),
    spi_flash_w25q128jv_block178 = (178U),
    spi_flash_w25q128jv_block179 = (179U),
    spi_flash_w25q128jv_block180 = (180U),
    spi_flash_w25q128jv_block181 = (181U),
    spi_flash_w25q128jv_block182 = (182U),
    spi_flash_w25q128jv_block183 = (183U),
    spi_flash_w25q128jv_block184 = (184U),
    spi_flash_w25q128jv_block185 = (185U),
    spi_flash_w25q128jv_block186 = (186U),
    spi_flash_w25q128jv_block187 = (187U),
    spi_flash_w25q128jv_block188 = (188U),
    spi_flash_w25q128jv_block189 = (189U),
    spi_flash_w25q128jv_block190 = (190U),
    spi_flash_w25q128jv_block191 = (191U),
    spi_flash_w25q128jv_block192 = (192U),
    spi_flash_w25q128jv_block193 = (193U),
    spi_flash_w25q128jv_block194 = (194U),
    spi_flash_w25q128jv_block195 = (195U),
    spi_flash_w25q128jv_block196 = (196U),
    spi_flash_w25q128jv_block197 = (197U),
    spi_flash_w25q128jv_block198 = (198U),
    spi_flash_w25q128jv_block199 = (199U),
    spi_flash_w25q128jv_block200 = (200U),
    spi_flash_w25q128jv_block201 = (201U),
    spi_flash_w25q128jv_block202 = (202U),
    spi_flash_w25q128jv_block203 = (203U),
    spi_flash_w25q128jv_block204 = (204U),
    spi_flash_w25q128jv_block205 = (205U),
    spi_flash_w25q128jv_block206 = (206U),
    spi_flash_w25q128jv_block207 = (207U),
    spi_flash_w25q128jv_block208 = (208U),
    spi_flash_w25q128jv_block209 = (209U),
    spi_flash_w25q128jv_block210 = (210U),
    spi_flash_w25q128jv_block211 = (211U),
    spi_flash_w25q128jv_block212 = (212U),
    spi_flash_w25q128jv_block213 = (213U),
    spi_flash_w25q128jv_block214 = (214U),
    spi_flash_w25q128jv_block215 = (215U),
    spi_flash_w25q128jv_block216 = (216U),
    spi_flash_w25q128jv_block217 = (217U),
    spi_flash_w25q128jv_block218 = (218U),
    spi_flash_w25q128jv_block219 = (219U),
    spi_flash_w25q128jv_block220 = (220U),
    spi_flash_w25q128jv_block221 = (221U),
    spi_flash_w25q128jv_block222 = (222U),
    spi_flash_w25q128jv_block223 = (223U),
    spi_flash_w25q128jv_block224 = (224U),
    spi_flash_w25q128jv_block225 = (225U),
    spi_flash_w25q128jv_block226 = (226U),
    spi_flash_w25q128jv_block227 = (227U),
    spi_flash_w25q128jv_block228 = (228U),
    spi_flash_w25q128jv_block229 = (229U),
    spi_flash_w25q128jv_block230 = (230U),
    spi_flash_w25q128jv_block231 = (231U),
    spi_flash_w25q128jv_block232 = (232U),
    spi_flash_w25q128jv_block233 = (233U),
    spi_flash_w25q128jv_block234 = (234U),
    spi_flash_w25q128jv_block235 = (235U),
    spi_flash_w25q128jv_block236 = (236U),
    spi_flash_w25q128jv_block237 = (237U),
    spi_flash_w25q128jv_block238 = (238U),
    spi_flash_w25q128jv_block239 = (239U),
    spi_flash_w25q128jv_block240 = (240U),
    spi_flash_w25q128jv_block241 = (241U),
    spi_flash_w25q128jv_block242 = (242U),
    spi_flash_w25q128jv_block243 = (243U),
    spi_flash_w25q128jv_block244 = (244U),
    spi_flash_w25q128jv_block245 = (245U),
    spi_flash_w25q128jv_block246 = (246U),
    spi_flash_w25q128jv_block247 = (247U),
    spi_flash_w25q128jv_block248 = (248U),
    spi_flash_w25q128jv_block249 = (249U),
    spi_flash_w25q128jv_block250 = (250U),
    spi_flash_w25q128jv_block251 = (251U),
    spi_flash_w25q128jv_block252 = (252U),
    spi_flash_w25q128jv_block253 = (253U),
    spi_flash_w25q128jv_block254 = (254U),
    spi_flash_w25q128jv_block255 = SPI_FLASH_W25Q128JV_MAX_BLOCK
} SPI_FLASH_W25Q128JV_BLOCK_e;

#define SPI_FLASH_W25Q128JV_FIRST_32KB  (0x00U)
#define SPI_FLASH_W25Q128JV_SECOND_32KB (0x80U)

typedef enum
{
    spi_flash_w25q128jv_first_32kb = SPI_FLASH_W25Q128JV_FIRST_32KB,
    spi_flash_w25q128jv_second_32kb = SPI_FLASH_W25Q128JV_SECOND_32KB
} SPI_FLASH_W25Q128JV_BLOCK_REGION_e;


#define SPI_FLASH_W25Q128JV_MAX_SECTOR (15U)

typedef enum
{
    spi_flash_w25q128jv_sector0 = (0U),
    spi_flash_w25q128jv_sector1 = (1U),
    spi_flash_w25q128jv_sector2 = (2U),
    spi_flash_w25q128jv_sector3 = (3U),
    spi_flash_w25q128jv_sector4 = (4U),
    spi_flash_w25q128jv_sector5 = (5U),
    spi_flash_w25q128jv_sector6 = (6U),
    spi_flash_w25q128jv_sector7 = (7U),
    spi_flash_w25q128jv_sector8 = (8U),
    spi_flash_w25q128jv_sector9 = (9U),
    spi_flash_w25q128jv_sector10 = (10U),
    spi_flash_w25q128jv_sector11 = (11U),
    spi_flash_w25q128jv_sector12 = (12U),
    spi_flash_w25q128jv_sector13 = (13U),
    spi_flash_w25q128jv_sector14 = (14U),
    spi_flash_w25q128jv_sector15 = SPI_FLASH_W25Q128JV_MAX_SECTOR
} SPI_FLASH_W25Q128JV_SECTOR_e;


#define SPI_FLASH_W25Q128JV_MAX_PAGE (15U)

typedef enum
{
    spi_flash_w25q128jv_page0 = (0U),
    spi_flash_w25q128jv_page1 = (1U),
    spi_flash_w25q128jv_page2 = (2U),
    spi_flash_w25q128jv_page3 = (3U),
    spi_flash_w25q128jv_page4 = (4U),
    spi_flash_w25q128jv_page5 = (5U),
    spi_flash_w25q128jv_page6 = (6U),
    spi_flash_w25q128jv_page7 = (7U),
    spi_flash_w25q128jv_page8 = (8U),
    spi_flash_w25q128jv_page9 = (9U),
    spi_flash_w25q128jv_page10 = (10U),
    spi_flash_w25q128jv_page11 = (11U),
    spi_flash_w25q128jv_page12 = (12U),
    spi_flash_w25q128jv_page13 = (13U),
    spi_flash_w25q128jv_page14 = (14U),
    spi_flash_w25q128jv_page15 = SPI_FLASH_W25Q128JV_MAX_PAGE
} SPI_FLASH_W25Q128JV_PAGE_e;


typedef enum
{
    spi_flash_w25q128jv_1byte = (1U),
    spi_flash_w25q128jv_2byte = (2U),
    spi_flash_w25q128jv_3byte = (3U),
    spi_flash_w25q128jv_4byte = (4U),
    spi_flash_w25q128jv_5byte = (5U),
    spi_flash_w25q128jv_6byte = (6U),
    spi_flash_w25q128jv_7byte = (7U),
    spi_flash_w25q128jv_8byte = (8U),
    spi_flash_w25q128jv_9byte = (9U),
    spi_flash_w25q128jv_10byte = (10U),
    spi_flash_w25q128jv_11byte = (11U),
    spi_flash_w25q128jv_12byte = (12U),
    spi_flash_w25q128jv_13byte = (13U),
    spi_flash_w25q128jv_14byte = (14U),
    spi_flash_w25q128jv_15byte = (15U),
    spi_flash_w25q128jv_16byte = (16U),
    spi_flash_w25q128jv_17byte = (17U),
    spi_flash_w25q128jv_18byte = (18U),
    spi_flash_w25q128jv_19byte = (19U),
    spi_flash_w25q128jv_20byte = (20U),
    spi_flash_w25q128jv_21byte = (21U),
    spi_flash_w25q128jv_22byte = (22U),
    spi_flash_w25q128jv_23byte = (23U),
    spi_flash_w25q128jv_24byte = (24U),
    spi_flash_w25q128jv_25byte = (25U),
    spi_flash_w25q128jv_26byte = (26U),
    spi_flash_w25q128jv_27byte = (27U),
    spi_flash_w25q128jv_28byte = (28U),
    spi_flash_w25q128jv_29byte = (29U),
    spi_flash_w25q128jv_30byte = (30U),
    spi_flash_w25q128jv_31byte = (31U),
    spi_flash_w25q128jv_32byte = (32U),
    spi_flash_w25q128jv_33byte = (33U),
    spi_flash_w25q128jv_34byte = (34U),
    spi_flash_w25q128jv_35byte = (35U),
    spi_flash_w25q128jv_36byte = (36U),
    spi_flash_w25q128jv_37byte = (37U),
    spi_flash_w25q128jv_38byte = (38U),
    spi_flash_w25q128jv_39byte = (39U),
    spi_flash_w25q128jv_40byte = (40U),
    spi_flash_w25q128jv_41byte = (41U),
    spi_flash_w25q128jv_42byte = (42U),
    spi_flash_w25q128jv_43byte = (43U),
    spi_flash_w25q128jv_44byte = (44U),
    spi_flash_w25q128jv_45byte = (45U),
    spi_flash_w25q128jv_46byte = (46U),
    spi_flash_w25q128jv_47byte = (47U),
    spi_flash_w25q128jv_48byte = (48U),
    spi_flash_w25q128jv_49byte = (49U),
    spi_flash_w25q128jv_50byte = (50U),
    spi_flash_w25q128jv_51byte = (51U),
    spi_flash_w25q128jv_52byte = (52U),
    spi_flash_w25q128jv_53byte = (53U),
    spi_flash_w25q128jv_54byte = (54U),
    spi_flash_w25q128jv_55byte = (55U),
    spi_flash_w25q128jv_56byte = (56U),
    spi_flash_w25q128jv_57byte = (57U),
    spi_flash_w25q128jv_58byte = (58U),
    spi_flash_w25q128jv_59byte = (59U),
    spi_flash_w25q128jv_60byte = (60U),
    spi_flash_w25q128jv_61byte = (61U),
    spi_flash_w25q128jv_62byte = (62U),
    spi_flash_w25q128jv_63byte = (63U),
    spi_flash_w25q128jv_64byte = (64U),
    spi_flash_w25q128jv_65byte = (65U),
    spi_flash_w25q128jv_66byte = (66U),
    spi_flash_w25q128jv_67byte = (67U),
    spi_flash_w25q128jv_68byte = (68U),
    spi_flash_w25q128jv_69byte = (69U),
    spi_flash_w25q128jv_70byte = (70U),
    spi_flash_w25q128jv_71byte = (71U),
    spi_flash_w25q128jv_72byte = (72U),
    spi_flash_w25q128jv_73byte = (73U),
    spi_flash_w25q128jv_74byte = (74U),
    spi_flash_w25q128jv_75byte = (75U),
    spi_flash_w25q128jv_76byte = (76U),
    spi_flash_w25q128jv_77byte = (77U),
    spi_flash_w25q128jv_78byte = (78U),
    spi_flash_w25q128jv_79byte = (79U),
    spi_flash_w25q128jv_80byte = (80U),
    spi_flash_w25q128jv_81byte = (81U),
    spi_flash_w25q128jv_82byte = (82U),
    spi_flash_w25q128jv_83byte = (83U),
    spi_flash_w25q128jv_84byte = (84U),
    spi_flash_w25q128jv_85byte = (85U),
    spi_flash_w25q128jv_86byte = (86U),
    spi_flash_w25q128jv_87byte = (87U),
    spi_flash_w25q128jv_88byte = (88U),
    spi_flash_w25q128jv_89byte = (89U),
    spi_flash_w25q128jv_90byte = (90U),
    spi_flash_w25q128jv_91byte = (91U),
    spi_flash_w25q128jv_92byte = (92U),
    spi_flash_w25q128jv_93byte = (93U),
    spi_flash_w25q128jv_94byte = (94U),
    spi_flash_w25q128jv_95byte = (95U),
    spi_flash_w25q128jv_96byte = (96U),
    spi_flash_w25q128jv_97byte = (97U),
    spi_flash_w25q128jv_98byte = (98U),
    spi_flash_w25q128jv_99byte = (99U),
    spi_flash_w25q128jv_100byte = (100U),
    spi_flash_w25q128jv_101byte = (101U),
    spi_flash_w25q128jv_102byte = (102U),
    spi_flash_w25q128jv_103byte = (103U),
    spi_flash_w25q128jv_104byte = (104U),
    spi_flash_w25q128jv_105byte = (105U),
    spi_flash_w25q128jv_106byte = (106U),
    spi_flash_w25q128jv_107byte = (107U),
    spi_flash_w25q128jv_108byte = (108U),
    spi_flash_w25q128jv_109byte = (109U),
    spi_flash_w25q128jv_110byte = (110U),
    spi_flash_w25q128jv_111byte = (111U),
    spi_flash_w25q128jv_112byte = (112U),
    spi_flash_w25q128jv_113byte = (113U),
    spi_flash_w25q128jv_114byte = (114U),
    spi_flash_w25q128jv_115byte = (115U),
    spi_flash_w25q128jv_116byte = (116U),
    spi_flash_w25q128jv_117byte = (117U),
    spi_flash_w25q128jv_118byte = (118U),
    spi_flash_w25q128jv_119byte = (119U),
    spi_flash_w25q128jv_120byte = (120U),
    spi_flash_w25q128jv_121byte = (121U),
    spi_flash_w25q128jv_122byte = (122U),
    spi_flash_w25q128jv_123byte = (123U),
    spi_flash_w25q128jv_124byte = (124U),
    spi_flash_w25q128jv_125byte = (125U),
    spi_flash_w25q128jv_126byte = (126U),
    spi_flash_w25q128jv_127byte = (127U),
    spi_flash_w25q128jv_128byte = (128U),
    spi_flash_w25q128jv_129byte = (129U),
    spi_flash_w25q128jv_130byte = (130U),
    spi_flash_w25q128jv_131byte = (131U),
    spi_flash_w25q128jv_132byte = (132U),
    spi_flash_w25q128jv_133byte = (133U),
    spi_flash_w25q128jv_134byte = (134U),
    spi_flash_w25q128jv_135byte = (135U),
    spi_flash_w25q128jv_136byte = (136U),
    spi_flash_w25q128jv_137byte = (137U),
    spi_flash_w25q128jv_138byte = (138U),
    spi_flash_w25q128jv_139byte = (139U),
    spi_flash_w25q128jv_140byte = (140U),
    spi_flash_w25q128jv_141byte = (141U),
    spi_flash_w25q128jv_142byte = (142U),
    spi_flash_w25q128jv_143byte = (143U),
    spi_flash_w25q128jv_144byte = (144U),
    spi_flash_w25q128jv_145byte = (145U),
    spi_flash_w25q128jv_146byte = (146U),
    spi_flash_w25q128jv_147byte = (147U),
    spi_flash_w25q128jv_148byte = (148U),
    spi_flash_w25q128jv_149byte = (149U),
    spi_flash_w25q128jv_150byte = (150U),
    spi_flash_w25q128jv_151byte = (151U),
    spi_flash_w25q128jv_152byte = (152U),
    spi_flash_w25q128jv_153byte = (153U),
    spi_flash_w25q128jv_154byte = (154U),
    spi_flash_w25q128jv_155byte = (155U),
    spi_flash_w25q128jv_156byte = (156U),
    spi_flash_w25q128jv_157byte = (157U),
    spi_flash_w25q128jv_158byte = (158U),
    spi_flash_w25q128jv_159byte = (159U),
    spi_flash_w25q128jv_160byte = (160U),
    spi_flash_w25q128jv_161byte = (161U),
    spi_flash_w25q128jv_162byte = (162U),
    spi_flash_w25q128jv_163byte = (163U),
    spi_flash_w25q128jv_164byte = (164U),
    spi_flash_w25q128jv_165byte = (165U),
    spi_flash_w25q128jv_166byte = (166U),
    spi_flash_w25q128jv_167byte = (167U),
    spi_flash_w25q128jv_168byte = (168U),
    spi_flash_w25q128jv_169byte = (169U),
    spi_flash_w25q128jv_170byte = (170U),
    spi_flash_w25q128jv_171byte = (171U),
    spi_flash_w25q128jv_172byte = (172U),
    spi_flash_w25q128jv_173byte = (173U),
    spi_flash_w25q128jv_174byte = (174U),
    spi_flash_w25q128jv_175byte = (175U),
    spi_flash_w25q128jv_176byte = (176U),
    spi_flash_w25q128jv_177byte = (177U),
    spi_flash_w25q128jv_178byte = (178U),
    spi_flash_w25q128jv_179byte = (179U),
    spi_flash_w25q128jv_180byte = (180U),
    spi_flash_w25q128jv_181byte = (181U),
    spi_flash_w25q128jv_182byte = (182U),
    spi_flash_w25q128jv_183byte = (183U),
    spi_flash_w25q128jv_184byte = (184U),
    spi_flash_w25q128jv_185byte = (185U),
    spi_flash_w25q128jv_186byte = (186U),
    spi_flash_w25q128jv_187byte = (187U),
    spi_flash_w25q128jv_188byte = (188U),
    spi_flash_w25q128jv_189byte = (189U),
    spi_flash_w25q128jv_190byte = (190U),
    spi_flash_w25q128jv_191byte = (191U),
    spi_flash_w25q128jv_192byte = (192U),
    spi_flash_w25q128jv_193byte = (193U),
    spi_flash_w25q128jv_194byte = (194U),
    spi_flash_w25q128jv_195byte = (195U),
    spi_flash_w25q128jv_196byte = (196U),
    spi_flash_w25q128jv_197byte = (197U),
    spi_flash_w25q128jv_198byte = (198U),
    spi_flash_w25q128jv_199byte = (199U),
    spi_flash_w25q128jv_200byte = (200U),
    spi_flash_w25q128jv_201byte = (201U),
    spi_flash_w25q128jv_202byte = (202U),
    spi_flash_w25q128jv_203byte = (203U),
    spi_flash_w25q128jv_204byte = (204U),
    spi_flash_w25q128jv_205byte = (205U),
    spi_flash_w25q128jv_206byte = (206U),
    spi_flash_w25q128jv_207byte = (207U),
    spi_flash_w25q128jv_208byte = (208U),
    spi_flash_w25q128jv_209byte = (209U),
    spi_flash_w25q128jv_210byte = (210U),
    spi_flash_w25q128jv_211byte = (211U),
    spi_flash_w25q128jv_212byte = (212U),
    spi_flash_w25q128jv_213byte = (213U),
    spi_flash_w25q128jv_214byte = (214U),
    spi_flash_w25q128jv_215byte = (215U),
    spi_flash_w25q128jv_216byte = (216U),
    spi_flash_w25q128jv_217byte = (217U),
    spi_flash_w25q128jv_218byte = (218U),
    spi_flash_w25q128jv_219byte = (219U),
    spi_flash_w25q128jv_220byte = (220U),
    spi_flash_w25q128jv_221byte = (221U),
    spi_flash_w25q128jv_222byte = (222U),
    spi_flash_w25q128jv_223byte = (223U),
    spi_flash_w25q128jv_224byte = (224U),
    spi_flash_w25q128jv_225byte = (225U),
    spi_flash_w25q128jv_226byte = (226U),
    spi_flash_w25q128jv_227byte = (227U),
    spi_flash_w25q128jv_228byte = (228U),
    spi_flash_w25q128jv_229byte = (229U),
    spi_flash_w25q128jv_230byte = (230U),
    spi_flash_w25q128jv_231byte = (231U),
    spi_flash_w25q128jv_232byte = (232U),
    spi_flash_w25q128jv_233byte = (233U),
    spi_flash_w25q128jv_234byte = (234U),
    spi_flash_w25q128jv_235byte = (235U),
    spi_flash_w25q128jv_236byte = (236U),
    spi_flash_w25q128jv_237byte = (237U),
    spi_flash_w25q128jv_238byte = (238U),
    spi_flash_w25q128jv_239byte = (239U),
    spi_flash_w25q128jv_240byte = (240U),
    spi_flash_w25q128jv_241byte = (241U),
    spi_flash_w25q128jv_242byte = (242U),
    spi_flash_w25q128jv_243byte = (243U),
    spi_flash_w25q128jv_244byte = (244U),
    spi_flash_w25q128jv_245byte = (245U),
    spi_flash_w25q128jv_246byte = (246U),
    spi_flash_w25q128jv_247byte = (247U),
    spi_flash_w25q128jv_248byte = (248U),
    spi_flash_w25q128jv_249byte = (249U)
/*  spi_flash_w25q128jv_250byte = (250U),
    spi_flash_w25q128jv_251byte = (251U),
    spi_flash_w25q128jv_252byte = (252U),
    spi_flash_w25q128jv_253byte = (253U),
    spi_flash_w25q128jv_254byte = (254U),
    spi_flash_w25q128jv_255byte = (255U)*/
} SPI_FLASH_W25Q128JV_DATA_LENGTH_e;

#define SPI_FLASH_W25Q128JV_PAGE_LENGTH           (249U)
#define SPI_FLASH_W25Q128JV_CHECK_BYTE (0xCCU)
#define SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH (4U)
#define SPI_FLASH_W25Q128JV_CRC_AND_TIME_LENGTH (6U)
#define SPI_FLASH_W25Q128JV_DMA_DATA_LENGTH (SPI_FLASH_W25Q128JV_PAGE_LENGTH + SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH + SPI_FLASH_W25Q128JV_CRC_AND_TIME_LENGTH)

typedef enum
{
    spi_flash_w25q128jv_operation_write = (0U),
    spi_flash_w25q128jv_operation_read = (1U),
    spi_flash_w25q128jv_operation_sector_erase = (2U),
    spi_flash_w25q128jv_operation_full_block_erase = (3U),
    spi_flash_w25q128jv_operation_chip_erase = (4U)
} SPI_FLASH_W25Q128JV_OPERATION_e;

#define SPI_FLASH_W25Q128JV_CRC_NOT_OK (0U)
#define SPI_FLASH_W25Q128JV_CRC_OK (1U)

typedef enum
{
    spi_flash_w25q128jv_crc_not_ok = SPI_FLASH_W25Q128JV_CRC_NOT_OK,
    spi_flash_w25q128jv_crc_ok = SPI_FLASH_W25Q128JV_CRC_OK
}SPI_FLASH_W25Q128JV_CRC_STATUS_e;

typedef struct
{
    SPI_FLASH_W25Q128JV_BLOCK_e block_e;
    SPI_FLASH_W25Q128JV_BLOCK_REGION_e block_region_e;
    SPI_FLASH_W25Q128JV_SECTOR_e sector_e;
    SPI_FLASH_W25Q128JV_PAGE_e page_e;
    SPI_FLASH_W25Q128JV_DATA_LENGTH_e data_length_e;
    SPI_FLASH_W25Q128JV_OPERATION_e operation_e;
    uint8_t data[SPI_FLASH_W25Q128JV_PAGE_LENGTH];
    uint32_t time_sec;
    SPI_FLASH_W25Q128JV_CRC_STATUS_e crc_status;
} SPI_FLASH_W25Q128JV_s;

typedef enum
{
    spi_flash_w25q128jv_no_fault = 0U,
    spi_flash_w25q128jv_chip_erase_fault = 1U,
    spi_flash_w25q128jv_full_block_erase_fault = 2U,
    spi_flash_w25q128jv_half_block_erase_fault = 3U,
    spi_flash_w25q128jv_sector_erase_fault = 4U,
    spi_flash_w25q128jv_page_write_fault = 5U,
    spi_flash_w25q128jv_page_read_fault = 6U,
    spi_flash_w25q128jv_reset_fault = 7U,
    spi_flash_w25q128jv_write_in_process_fault = 8U,
    spi_flash_w25q128jv_out_of_range_memeory_address = 9U
} SPI_FLASH_W25Q128JV_FAULT_TYPE_e;

typedef struct
{
    uint16_t status_register_protect;
    uint16_t sector_protect;
    uint16_t top_bottom_protect;
    uint16_t block_proctect_bits;
    uint16_t write_enable_latch;
    uint16_t write_in_process;
} SPI_FLASH_W25Q128JV_status_register1_s;

typedef struct
{
    uint16_t suspend_status;
    uint16_t complement_protect;
    uint16_t security_register_lock_bits;
    uint16_t quad_enable;
    uint16_t status_register_lock;
} SPI_FLASH_W25Q128JV_status_register2_s;

typedef struct
{
    uint16_t output_driver_strength;
    uint16_t write_protect_selection;
} SPI_FLASH_W25Q128JV_status_register3_s;

/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t* spi_flash_w25q128jv_dma_busy_status(void);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_chip_erase(void);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_full_block_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_half_block_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_sector_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_page_write(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_page_read(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s);
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_reset(void);
#endif /* SPI_FLASH_W25Q128JV_H */
/*================== END MODULE ==============================================*/
