#pragma once

namespace freezone { namespace protocol {

enum curve_id
{
   quadratic,
   bounded,
   linear,
   square_root,
   convergent_linear,
   convergent_square_root
};

} } // freezone::utilities


FC_REFLECT_ENUM(
   freezone::protocol::curve_id,
   (quadratic)
   (bounded)
   (linear)
   (square_root)
   (convergent_linear)
   (convergent_square_root)
)
