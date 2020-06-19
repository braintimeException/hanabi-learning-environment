#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../hanabi_lib/hanabi_move.h"
#include "representations.h"

namespace py = pybind11;
namespace hle = hanabi_learning_env;

void wrap_hanabi_move(py::module& m) {
  py::class_<hle::HanabiMove> hanabi_move(m, "HanabiMove");

  py::enum_<hle::HanabiMove::Type>(hanabi_move, "Type")
    .value("kInvalid", hle::HanabiMove::Type::kInvalid)
    .value("kPlay", hle::HanabiMove::Type::kPlay)
    .value("kDiscard", hle::HanabiMove::Type::kDiscard)
    .value("kRevealColor", hle::HanabiMove::Type::kRevealColor)
    .value("kRevealRank", hle::HanabiMove::Type::kRevealRank)
    .value("kDeal", hle::HanabiMove::Type::kDeal)
    .export_values();

  hanabi_move
    .def(py::init<hle::HanabiMove::Type, int8_t, int8_t, int8_t, int8_t>(),
         py::arg("move_type") = hle::HanabiMove::Type::kInvalid,
         py::arg("card_index") = -1,
         py::arg("target_offset") = -1,
         py::arg("color") = -1,
         py::arg("rank") = -1
    )
    .def(py::init<hle::HanabiMove::Type,
                  int8_t,
                  int8_t,
                  hle::HanabiCard::ColorType,
                  hle::HanabiCard::RankType>(),
         py::arg("move_type") = hle::HanabiMove::Type::kInvalid,
         py::arg("card_index") = -1,
         py::arg("target_offset") = -1,
         py::arg("color") = hle::HanabiCard::ColorType::kUnknownColor,
         py::arg("rank") = hle::HanabiCard::RankType::kUnknownRank
    )
    .def(py::init<hle::HanabiMove::Type, int8_t, int8_t, hle::HanabiCard>(),
         py::arg("move_type") = hle::HanabiMove::Type::kInvalid,
         py::arg("card_index") = -1,
         py::arg("target_offset") = -1,
         py::arg("card") = hle::HanabiCard()
    )
    .def("__eq__",
         &hle::HanabiMove::operator==,
         "Tests whether two moves are functionally equivalent.")
    .def_property_readonly("move_type", &hle::HanabiMove::MoveType)
    .def("is_valid", &hle::HanabiMove::IsValid)
    .def_property_readonly(
        "card_index",
        &hle::HanabiMove::CardIndex,
        "Index of the card this move applies to. Used with Play/Discard moves."
    )
    .def_property_readonly(
        "target_offset",
        &hle::HanabiMove::TargetOffset,
        "An offset from the acting player. Used with RevealXYZ moves"
    )
    .def_property_readonly("color", &hle::HanabiMove::Color)
    .def_property_readonly("rank", &hle::HanabiMove::Rank)

    .def("__str__", &hle::HanabiMove::ToString)
    .def("__repr__", &hanabi_move_repr) 

    .doc() = "5 types of moves: "
      "\"Play\" card_index    of card in player hand\n"
      "\"Discard\" card_index    of card in player hand\n"
      "\"RevealColor\" target_offset color    hints to player all cards of color\n"
      "\"RevealRank\" target_offset rank    hints to player all cards of given rank\n"
      "NOTE: RevealXYZ target_offset field is an offset from the acting player\n"
      "\"Deal\" color rank    deal card with color and rank\n"
      "\"Invalid\"   move is not valid\n";
}
