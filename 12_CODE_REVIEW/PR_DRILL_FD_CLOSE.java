// PR DRILL — FD (Fixed Deposit) waqt se pehle todna
// PR: "Add premature FD closure API"   ·   Author: junior dev   ·   Reviewer: tu
// Kaam: user apni FD time se pehle todta hai. Paisa uske savings account me jaata hai, thoda penalty kat ke.
// Tu upar se neeche padh, jahan dikkat dikhe wo line + kya hoga + sahi kya, bol de. Aakhir me FAISLA.

@RestController
@RequestMapping("/api/fd")
@RequiredArgsConstructor
public class FdController {

    private final FdService fdService;

    @PostMapping("/{fdId}/close")
    public ResponseEntity<String> close(@PathVariable Long fdId, @AuthenticationPrincipal User user) {
        return ResponseEntity.ok(fdService.process(fdId, user.getId()));
    }
}

@Service
@RequiredArgsConstructor
@Slf4j
public class FdService {

    private final FdRepository fdRepo;
    private final AccountRepository accountRepo;
    private final CoreBankingClient coreBanking;      // bahar ka system, savings me paisa daalta

    @Transactional
    public String process(Long fdId, Long userId) {
        FixedDeposit fd = fdRepo.findById(fdId).orElseThrow(() -> new NotFoundException("FD nahi mili"));

        if (!fd.getUserId().equals(userId)) {
            throw new ForbiddenException("ye FD tumhari nahi");
        }

        if (fd.getStatus() == FdStatus.ACTIVE) {
            BigDecimal penalty = fd.getPrincipal().multiply(new BigDecimal("0.01"));
            BigDecimal payout = fd.getPrincipal().add(fd.getInterestEarned()).subtract(penalty);

            try {
                coreBanking.creditSavings(fd.getSavingsAccountId(), payout);
            } catch (Exception e) {
                log.error("credit fail fdId={}", fdId);
                return "FAILED";
            }

            fd.setStatus(FdStatus.CLOSED);
            fd.setClosedAt(Instant.now());
            fdRepo.save(fd);

            Account acc = accountRepo.findById(fd.getSavingsAccountId()).get();
            log.info("FD closed fdId={} payout={}", fdId, payout);
        }

        return "CLOSED";
    }
}
