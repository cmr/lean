example (a b : nat) : (λ x, a + x) 0 = b + 1 + a :=
begin
  -- TODO(Leo): improve pattern syntax.
  find (λ v, v + 1) { change nat.succ b },
  guard_target (λ x, a + x) 0 = nat.succ b + a,
  admit
end
