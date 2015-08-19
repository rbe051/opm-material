// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
// vi: set et ts=4 sw=4 sts=4:
/*
  Copyright (C) 2015 by Andreas Lauser
  Copyright (C) 2015 by IRIS AS

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 * \file
 * \copydoc Opm::EclEpsTwoPhaseLawPoints
 */
#ifndef OPM_ECL_EPS_SCALING_POINTS_HPP
#define OPM_ECL_EPS_SCALING_POINTS_HPP

#include "EclEpsConfig.hpp"

#if HAVE_OPM_PARSER
#include <opm/parser/eclipse/Deck/Deck.hpp>
#include <opm/parser/eclipse/EclipseState/EclipseState.hpp>
#endif

#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

namespace Opm {
/*!
 * \brief Collects all grid properties which are relevant for end point scaling.
 *
 * This class is used for both, the drainage and the imbibition variants of the ECL
 * keywords.
 */
class EclEpsGridProperties
{
    typedef std::vector<int> IntData;
    typedef std::vector<double> DoubleData;

public:
#if HAVE_OPM_PARSER
    void initFromDeck(Opm::DeckConstPtr deck,
                      Opm::EclipseStateConstPtr eclState,
                      bool useImbibition)
    {
        std::string kwPrefix = useImbibition?"I":"";

        if (useImbibition)
            satnum = &eclState->getIntGridProperty("IMBNUM")->getData();
        else
            satnum = &eclState->getIntGridProperty("SATNUM")->getData();

        retrieveGridPropertyData_(&swl, deck, eclState, kwPrefix+"SWL");
        retrieveGridPropertyData_(&sgl, deck, eclState, kwPrefix+"SGL");
        retrieveGridPropertyData_(&swcr, deck, eclState, kwPrefix+"SWCR");
        retrieveGridPropertyData_(&sgcr, deck, eclState, kwPrefix+"SGCR");
        retrieveGridPropertyData_(&sowcr, deck, eclState, kwPrefix+"SOWCR");
        retrieveGridPropertyData_(&sogcr, deck, eclState, kwPrefix+"SOGCR");
        retrieveGridPropertyData_(&swu, deck, eclState, kwPrefix+"SWU");
        retrieveGridPropertyData_(&sgu, deck, eclState, kwPrefix+"SGU");
        retrieveGridPropertyData_(&pcw, deck, eclState, kwPrefix+"PCW");
        retrieveGridPropertyData_(&pcg, deck, eclState, kwPrefix+"PCG");
        retrieveGridPropertyData_(&krw, deck, eclState, kwPrefix+"KRW");
        retrieveGridPropertyData_(&kro, deck, eclState, kwPrefix+"KRO");
        retrieveGridPropertyData_(&krg, deck, eclState, kwPrefix+"KRG");
    }
#endif

    const IntData* satnum;

    const DoubleData* swl;
    const DoubleData* sgl;
    const DoubleData* swcr;
    const DoubleData* sgcr;
    const DoubleData* sowcr;
    const DoubleData* sogcr;
    const DoubleData* swu;
    const DoubleData* sgu;
    const DoubleData* pcw;
    const DoubleData* pcg;
    const DoubleData* krw;
    const DoubleData* kro;
    const DoubleData* krg;

private:
#if HAVE_OPM_PARSER
    // this method makes sure that a grid property is not created if it is not explicitly
    // mentioned in the deck. (saves memory.)
    void retrieveGridPropertyData_(const DoubleData **data,
                                   Opm::DeckConstPtr deck,
                                   Opm::EclipseStateConstPtr eclState,
                                   const std::string& properyName)
    {
        (*data) = 0;
        if (eclState->hasDoubleGridProperty(properyName))
            (*data) = &eclState->getDoubleGridProperty(properyName)->getData();
    }
#endif
};

/*!
 * \brief This structure represents all values which can be possibly used as scaling
 *        points in the endpoint scaling code.
 *
 * Depending on the exact configuration, some of these quantities are not used as actual
 * scaling points. It is easier to extract all of them at once, though.
 */
template <class Scalar>
struct EclEpsScalingPointsInfo
{
    // connate saturations
    Scalar Swl; // oil
    Scalar Sgl; // gas
    Scalar Sowl; // oil for the oil-water system
    Scalar Sogl; // oil for the gas-oil system

    // critical water and gas saturations
    Scalar Swcr; // oil
    Scalar Sgcr; // gas
    Scalar Sowcr; // oil for the oil-water system
    Scalar Sogcr; // oil for the gas-oil system

    // maximum saturations
    Scalar Swu; // oil
    Scalar Sgu; // gas
    Scalar Sowu; // oil for the oil-water system
    Scalar Sogu; // oil for the gas-oil system

    // maximum capillary pressures
    Scalar maxPcow; // maximum capillary pressure of the oil-water system
    Scalar maxPcgo; // maximum capillary pressure of the gas-oil system

    // maximum relative permabilities
    Scalar maxKrw; // maximum relative permability of water
    Scalar maxKrow; // maximum relative permability of oil in the oil-water system
    Scalar maxKrog; // maximum relative permability of oil in the gas-oil system
    Scalar maxKrg; // maximum relative permability of gas

    void print() const
    {
        std::cout << "    Swl: " << Swl << "\n"
                  << "    Sgl: " << Sgl << "\n"
                  << "    Sowl: " << Sowl << "\n"
                  << "    Sogl: " << Sogl << "\n"
                  << "    Swcr: " << Swcr << "\n"
                  << "    Sgcr: " << Sgcr << "\n"
                  << "    Sowcr: " << Sowcr << "\n"
                  << "    Sogcr: " << Sogcr << "\n"
                  << "    Swu: " << Swu << "\n"
                  << "    Sgu: " << Sgu << "\n"
                  << "    Sowu: " << Sowu << "\n"
                  << "    Sogu: " << Sogu << "\n"
                  << "    maxPcow: " << maxPcow << "\n"
                  << "    maxPcgo: " << maxPcgo << "\n"
                  << "    maxKrw: " << maxKrw << "\n"
                  << "    maxKrg: " << maxKrg << "\n"
                  << "    maxKrow: " << maxKrow << "\n"
                  << "    maxKrog: " << maxKrog << "\n";
    }

#if HAVE_OPM_PARSER
    /*!
     * \brief Extract the values of the unscaled scaling parameters.
     *
     * I.e., the values which are used for the nested Fluid-Matrix interactions and which
     * are produced by them.
     */
    void extractUnscaled(Opm::DeckConstPtr deck,
                         Opm::EclipseStateConstPtr eclState,
                         int satRegionIdx)
    {
        // TODO: support for the SOF2/SOF3 keyword family
        const std::vector<SwofTable>& swofTables = eclState->getSwofTables();
        const std::vector<SgofTable>& sgofTables = eclState->getSgofTables();
        const std::vector<SwfnTable>& swfnTables = eclState->getSwfnTables();
        const std::vector<SgfnTable>& sgfnTables = eclState->getSgfnTables();
        const std::vector<Sof3Table>& sof3Tables = eclState->getSof3Tables();

        bool family1 = !sgofTables.empty() && !swofTables.empty();
        bool family2 = !swfnTables.empty() && !sgfnTables.empty() && !sof3Tables.empty();

        if (family1)
        {
            const auto& swofTable = swofTables[satRegionIdx];
            const auto& sgofTable = sgofTables[satRegionIdx];

        // connate saturations
        Swl = swofTable.getSwColumn().front();
        Sowl = 1.0 - swofTable.getSwColumn().back();
        Sgl = sgofTable.getSgColumn().front();
        Sogl = 1.0 - sgofTable.getSgColumn().back();

        // critical water saturation
        for (unsigned rowIdx = 0; rowIdx < swofTable.numRows(); ++ rowIdx) {
            if (swofTable.getKrwColumn()[rowIdx] > 0) {
                assert(rowIdx > 0);
                Swcr = swofTable.getSwColumn()[rowIdx - 1];
                break;
            };
        }

        // critical oil saturation of oil-water system
        for (int rowIdx = swofTable.numRows() - 1; rowIdx >= 0; -- rowIdx) {
            if (swofTable.getKrowColumn()[rowIdx] > 0) {
                assert(rowIdx < (int) swofTable.numRows() - 1);
                Sowcr = 1.0 - swofTable.getSwColumn()[rowIdx + 1];
                break;
            };
        }

        // critical gas saturation
        for (unsigned rowIdx = 0; rowIdx < sgofTable.numRows(); ++ rowIdx) {
            if (sgofTable.getKrgColumn()[rowIdx] > 0) {
                assert(rowIdx > 0);
                Sgcr = sgofTable.getSgColumn()[rowIdx - 1];
                break;
            };
        }

        // critical oil saturation of gas-oil system
        for (int rowIdx = sgofTable.numRows() - 1; rowIdx >= 0; -- rowIdx) {
            if (sgofTable.getKrogColumn()[rowIdx] > 0) {
                assert(rowIdx < (int) sgofTable.numRows() - 1);
                Sogcr = 1.0 - sgofTable.getSgColumn()[rowIdx + 1];
                break;
            };
        }

        // maximum saturations
        Swu = swofTable.getSwColumn().back();
        Sowu = 1.0 - swofTable.getSwColumn().front();
        Sgu = sgofTable.getSgColumn().back();
        Sogu = 1.0 - sgofTable.getSgColumn().front();

        // maximum capillary pressures
        maxPcow = swofTable.getPcowColumn().front();
        maxPcgo = sgofTable.getPcogColumn().back();

        // maximum relative permeabilities
        maxKrw = swofTable.getKrwColumn().back();
        maxKrow = swofTable.getKrowColumn().front();

        maxKrg = sgofTable.getKrgColumn().back();
        maxKrog = sgofTable.getKrogColumn().front();
        }
        else if (family2)
        {
            const auto& swfnTable = swfnTables[satRegionIdx];
            const auto& sof3Table = sof3Tables[satRegionIdx];
            const auto& sgfnTable = sgfnTables[satRegionIdx];

            // connate saturations
            Swl = swfnTable.getSwColumn().front();
            Sowl = sof3Table.getSoColumn().front() + Sgl;
            Sgl = sgfnTable.getSgColumn().front();
            Sogl = sof3Table.getSoColumn().front() + Swl;

            // critical water saturation
            for (unsigned rowIdx = 0; rowIdx < swfnTable.numRows(); ++ rowIdx) {
                if (swfnTable.getKrwColumn()[rowIdx] > 0) {
                    assert(rowIdx > 0);
                    Swcr = swfnTable.getSwColumn()[rowIdx - 1];
                    break;
                };
            }

            // critical oil saturation of oil-water system
            for (int rowIdx = 0 ; rowIdx < sof3Table.numRows(); ++ rowIdx) {
                if (sof3Table.getKrowColumn()[rowIdx] > 0) {
                    assert(rowIdx > 0);
                    Sowcr = sof3Table.getSoColumn()[rowIdx - 1];
                    break;
                };
            }

            // critical oil saturation of gas-oil system
            for (int rowIdx = 0 ; rowIdx < sof3Table.numRows(); ++ rowIdx) {
                if (sof3Table.getKrogColumn()[rowIdx] > 0) {
                    assert(rowIdx > 0);
                    Sogcr = sof3Table.getSoColumn()[rowIdx - 1];
                    break;
                };
            }

            // critical gas saturation
            for (unsigned rowIdx = 0; rowIdx < sgfnTable.numRows(); ++ rowIdx) {
                if (sgfnTable.getKrgColumn()[rowIdx] > 0) {
                    assert(rowIdx > 0);
                    Sgcr = sgfnTable.getSgColumn()[rowIdx - 1];
                    break;
                };
            }
            // maximum saturations
            Swu = swfnTable.getSwColumn().back();
            Sowu = sof3Table.getSoColumn().back();
            assert(Sowu == 1 - swfnTable.getSwColumn().front());
            Sgu = sgfnTable.getSgColumn().back();
            Sogu = 1 - sgfnTable.getSgColumn().front();

            // maximum capillary pressures
            maxPcow = swfnTable.getPcowColumn().front();
            maxPcgo = sgfnTable.getPcogColumn().back();

            // maximum relative permeabilities
            maxKrw = swfnTable.getKrwColumn().back();
            maxKrow = sof3Table.getKrowColumn().back();

            maxKrg = sgfnTable.getKrgColumn().back();
            maxKrog = sof3Table.getKrogColumn().back();

            assert(maxKrw == maxKrg);

        } else {
            throw std::domain_error("No valid saturation keyword family specified");
        }

    }
#endif

    /*!
     * \brief Extract the values of the scaled scaling parameters.
     *
     * I.e., the values which are "seen" by the physical model.
     */
    void extractScaled(const EclEpsGridProperties& epsProperties, int cartesianCellIdx)
    {
        // overwrite the unscaled values with the values for the cell if it is
        // explicitly specified by the corresponding keyword.
        extractGridPropertyValue_(Swl, epsProperties.swl, cartesianCellIdx);
        extractGridPropertyValue_(Sgl, epsProperties.sgl, cartesianCellIdx);
        extractGridPropertyValue_(Swcr, epsProperties.swcr, cartesianCellIdx);
        extractGridPropertyValue_(Sgcr, epsProperties.sgcr, cartesianCellIdx);
        extractGridPropertyValue_(Sowcr, epsProperties.sowcr, cartesianCellIdx);
        extractGridPropertyValue_(Sogcr, epsProperties.sogcr, cartesianCellIdx);
        extractGridPropertyValue_(Swu, epsProperties.swu, cartesianCellIdx);
        extractGridPropertyValue_(Sgu, epsProperties.sgu, cartesianCellIdx);

        extractGridPropertyValue_(maxPcow, epsProperties.pcw, cartesianCellIdx);
        extractGridPropertyValue_(maxPcgo, epsProperties.pcg, cartesianCellIdx);

        extractGridPropertyValue_(maxKrw, epsProperties.krw, cartesianCellIdx);
        extractGridPropertyValue_(maxKrg, epsProperties.krg, cartesianCellIdx);

        // quite likely that's wrong!
        extractGridPropertyValue_(maxKrow, epsProperties.kro, cartesianCellIdx);
        extractGridPropertyValue_(maxKrog, epsProperties.kro, cartesianCellIdx);
    }

private:
    void extractGridPropertyValue_(Scalar& targetValue,
                                   const std::vector<double>* propData,
                                   int cartesianCellIdx)
    {
        if (!propData)
            return;

        targetValue = (*propData)[cartesianCellIdx];
    }
};

/*!
 * \ingroup FluidMatrixInteractions
 *
 * \brief Represents the points on the X and Y axis to be scaled if endpoint scaling is
 *        used.
 */
template <class Scalar>
class EclEpsScalingPoints
{
public:
    /*!
     * \brief Assigns the scaling points which actually ought to be used.
     */
    void init(const EclEpsScalingPointsInfo<Scalar>& epsInfo,
              const EclEpsConfig& config,
              EclTwoPhaseSystemType epsSystemType)
    {
        if (epsSystemType == EclOilWaterSystem) {
            // saturation scaling for capillary pressure
            saturationPcPoints_[0] = epsInfo.Swl;
            saturationPcPoints_[1] = epsInfo.Swu;

            // krw saturation scaling endpoints
            if (config.enableThreePointKrSatScaling()) {
                saturationKrwPoints_[0] = epsInfo.Swcr;
                saturationKrwPoints_[1] = 1.0 - epsInfo.Sowcr - epsInfo.Sgl;
                saturationKrwPoints_[2] = epsInfo.Swu;
            }
            else {
                saturationKrwPoints_[0] = epsInfo.Swcr;
                saturationKrwPoints_[1] = epsInfo.Swu;
            }

            // krn saturation scaling endpoints (with the non-wetting phase being oil).
            // because opm-material specifies non-wetting phase relperms in terms of the
            // wetting phase saturations, the code here uses 1 minus the values specified
            // by the Eclipse TD and the order of the scaling points is reversed
            if (config.enableThreePointKrSatScaling()) {
                saturationKrnPoints_[2] = 1.0 - epsInfo.Sowcr;
                saturationKrnPoints_[1] = epsInfo.Swcr + epsInfo.Sgl;
                saturationKrnPoints_[0] = epsInfo.Swl + epsInfo.Sgl;
            }
            else {
                saturationKrnPoints_[1] = 1 - epsInfo.Sowcr;
                saturationKrnPoints_[0] = epsInfo.Swl + epsInfo.Sgl;
            }

            maxPcnw_ = epsInfo.maxPcow;
            maxKrw_ = epsInfo.maxKrw;
            maxKrn_ = epsInfo.maxKrow;
        }
        else {
            assert(epsSystemType == EclGasOilSystem);

            // saturation scaling for capillary pressure
            saturationPcPoints_[0] = 1.0 - epsInfo.Sgu;
            saturationPcPoints_[1] = 1.0 - epsInfo.Sgl;

            // krw saturation scaling endpoints
            if (config.enableThreePointKrSatScaling()) {
                saturationKrwPoints_[0] = epsInfo.Sogcr;
                saturationKrwPoints_[1] = 1 - epsInfo.Sgcr - epsInfo.Swl;
                saturationKrwPoints_[2] = 1 - epsInfo.Swl - epsInfo.Sgl;
            }
            else {
                saturationKrwPoints_[0] = epsInfo.Sogcr;
                saturationKrwPoints_[1] = 1 - epsInfo.Swl - epsInfo.Sgl;
            }

            // krn saturation scaling endpoints (with the non-wetting phase being gas).
            // because opm-material specifies non-wetting phase relperms in terms of the
            // wetting phase saturations, the code here uses 1 minus the values specified
            // by the Eclipse TD and the order of the scaling points is reversed
            if (config.enableThreePointKrSatScaling()) {
                saturationKrnPoints_[2] = 1.0 - epsInfo.Sgcr;
                saturationKrnPoints_[1] = epsInfo.Sogcr + epsInfo.Swl;
                saturationKrnPoints_[0] = 1.0 - epsInfo.Sgu;
            }
            else {
                saturationKrnPoints_[1] = 1.0 - epsInfo.Sgcr;
                saturationKrnPoints_[0] = 1.0 - epsInfo.Sgu;
            }

            maxPcnw_ = epsInfo.maxPcgo;
            maxKrw_ = epsInfo.maxKrog;
            maxKrn_ = epsInfo.maxKrg;
        }
    }

    /*!
     * \brief Sets an saturation value for capillary pressure saturation scaling
     */
    void setSaturationPcPoint(int pointIdx, Scalar value)
    { saturationPcPoints_[pointIdx] = value; }

    /*!
     * \brief Returns the points used for capillary pressure saturation scaling
     */
    const std::array<Scalar, 2>& saturationPcPoints() const
    { return saturationPcPoints_; }

    /*!
     * \brief Sets an saturation value for wetting-phase relperm saturation scaling
     */
    void setSaturationKrwPoint(int pointIdx, Scalar value)
    { saturationKrwPoints_[pointIdx] = value; }

    /*!
     * \brief Returns the points used for wetting phase relperm saturation scaling
     */
    const std::array<Scalar, 3>& saturationKrwPoints() const
    { return saturationKrwPoints_; }

    /*!
     * \brief Sets an saturation value for non-wetting phase relperm saturation scaling
     */
    void setSaturationKrnPoint(int pointIdx, Scalar value)
    { saturationKrnPoints_[pointIdx] = value; }

    /*!
     * \brief Returns the points used for non-wetting phase relperm saturation scaling
     */
    const std::array<Scalar, 3>& saturationKrnPoints() const
    { return saturationKrnPoints_; }

    /*!
     * \brief Sets the maximum capillary pressure
     */
    void setMaxPcnw(Scalar value)
    { maxPcnw_ = value; }

    /*!
     * \brief Returns the maximum capillary pressure
     */
    Scalar maxPcnw() const
    { return maxPcnw_; }

    /*!
     * \brief Sets the maximum wetting phase relative permeability
     */
    void setMaxKrw(Scalar value)
    { maxKrw_ = value; }

    /*!
     * \brief Returns the maximum wetting phase relative permeability
     */
    Scalar maxKrw() const
    { return maxKrw_; }

    /*!
     * \brief Sets the maximum wetting phase relative permeability
     */
    void setMaxKrn(Scalar value)
    { maxKrn_ = value; }

    /*!
     * \brief Returns the maximum wetting phase relative permeability
     */
    Scalar maxKrn() const
    { return maxKrn_; }

    void print() const
    {
        std::cout << "    saturationKrnPoints_[0]: " << saturationKrnPoints_[0] << "\n"
                  << "    saturationKrnPoints_[1]: " << saturationKrnPoints_[1] << "\n"
                  << "    saturationKrnPoints_[2]: " << saturationKrnPoints_[2] << "\n";
    }

private:
    // The the points used for the "y-axis" scaling of capillary pressure
    Scalar maxPcnw_;

    // The the points used for the "y-axis" scaling of wetting phase relative permability
    Scalar maxKrw_;

    // The the points used for the "y-axis" scaling of non-wetting phase relative permability
    Scalar maxKrn_;

    // The the points used for saturation ("x-axis") scaling of capillary pressure
    std::array<Scalar, 2> saturationPcPoints_;

    // The the points used for saturation ("x-axis") scaling of wetting phase relative permeability
    std::array<Scalar, 3> saturationKrwPoints_;

    // The the points used for saturation ("x-axis") scaling of non-wetting phase relative permeability
    std::array<Scalar, 3> saturationKrnPoints_;
};

} // namespace Opm

#endif
